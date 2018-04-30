#include "cStateManager.h"
#include "iEntity.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cSceneManager.h"
#include "cGameObject.h"
#include "iState.h"

extern sScene* g_pCurrentScene;

void cStateManager::determineStateBehaviours(std::vector<cGameObject*>& objects) {

	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->theAIState != NULL) {
			//of the type is a follower make sure to check all the objects in the scene to see what one is closest
			eStateType type = objects[i]->theAIState->getStateType();

			if (type == eStateType::FOLLOWER) {
				//have all the objects in the same vector
				std::vector<cGameObject*> theCopyVec = objects;
				std::vector<glm::vec3> thePositions;
				//sort the vector and get the closest one 
				for (int copyIndex = 0; copyIndex < theCopyVec.size(); copyIndex++) {
					if (copyIndex == i) {
						continue;
					}

					thePositions.push_back(theCopyVec[copyIndex]->position);
				}

				glm::vec3 closest(0.0f);
				float distance = 0.0f;

				//if it's the only enemy left in the vector jsut test against the player
				if (thePositions.size() == 0) {
					//perform action against the player
					objects[i]->theAIState->performAction(g_pCurrentScene->currentPlayer->thePlayerObject, objects[i], (float)1 / 60);
					continue;
				}

				//otherwise find the closest enemy
				distance = abs(glm::distance(objects[i]->position, thePositions[0]));
				//get the closest position
				for (int distIndex = 1; distIndex < thePositions.size(); distIndex++) {
					float currDist = abs(glm::distance(objects[i]->position, thePositions[distIndex]));

					if (currDist < distance) {
						distance = currDist;
						closest = thePositions[distIndex];
					}
				}

				//test against closest enemy
				for (int objectIndex = 0; objectIndex < theCopyVec.size(); objectIndex++) {
					//find the closest enemy adn perform an action
					if (closest == theCopyVec[objectIndex]->position) {
						objects[i]->theAIState->performAction(theCopyVec[objectIndex], objects[i], (float)1 / 60);
						break;
					}
				}

			}
			objects[i]->theAIState->performAction(g_pCurrentScene->currentPlayer->thePlayerObject, objects[i], (float)1 / 60);
		}
	}
}
