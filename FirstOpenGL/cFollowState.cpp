#include "cFollowState.h"

#include "cGameObject.h"
#include <glm\glm.hpp>
#include "cMathHelper.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cSceneManager.h"
#include "iEntity.h"
#include "cAnimationManager.h"

extern sScene* g_pCurrentScene;

cFollowState::cFollowState() {
	this->mAction = eActionType::IDLE;
	this->mState = eStateType::FOLLOWER;
	this->visionDistance = 7.0f;
	this->chaseThreshold = 14.0f;
	this->speed = 0.0f;
	float timeInRadius = 0.0f;
}

const glm::vec4 stateGreen(0.0f, 1.f, 0.f, 1.0f);
const glm::vec4 stateTeal(0.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 stateBlack(0.0f, 0.0f, 0.0f, 1.0f);

void cFollowState::performAction(cGameObject* player, cGameObject* me, float deltaTime) {
	float enemySpeed = 2.0f;
	//enemy direction
	Matrix RotationMatrix = IDENTITY_MATRIX;
	RotateAboutY(&RotationMatrix, me->orientation2.y);
	glm::vec3 forwardVector = glm::vec3(0.0f, 0.0f, 1.0f);
	forwardVector = VectorMultiplyMatrices(&RotationMatrix, &forwardVector);

	////player direction
	//RotationMatrix = IDENTITY_MATRIX;
	//RotateAboutY(&RotationMatrix, player->orientation2.y);
	//glm::vec3 playerforwardVector = glm::vec3(1.0f, 0.0f, 0.0f);
	//playerforwardVector = VectorMultiplyMatrices(&RotationMatrix, &playerforwardVector);

	////update the player forward vector
	//g_pCurrentScene->currentPlayer->playerForward = playerforwardVector;

	float playerLookingAtMe = glm::dot(forwardVector, g_pCurrentScene->currentPlayer->playerForward);

	//determine what way the player is facing
	//if (playerLookingAtMe < 0.0f) {
	//	this->mAction = ActionType::EVADE;
	//	this->isBehindPlayer = false;
	//}

	//FOLLOW NO MATTER WHAT WHILE IN RANGE
	if (playerLookingAtMe < 0.0f) {
		this->mAction = eActionType::FOLLOW;
		this->isBehindPlayer = false;
	}
	else if (playerLookingAtMe > 0.0f) {
		this->mAction = eActionType::FOLLOW;
		this->isBehindPlayer = true;
	}

	float distanceFromPlayer = abs(glm::distance(me->position, player->position));

	//determine what action to perform
	if (distanceFromPlayer > chaseThreshold) {
		this->mAction = eActionType::IDLE;
	}

	if (this->mAction == eActionType::EVADE) {
		glm::vec3 difVector = glm::vec3((player->position.x - me->position.x),
			(player->position.y - me->position.y),
			(player->position.z - me->position.z)
		);
		//reverse the forward vector
		forwardVector = -forwardVector;

		float forwardMag = GetMag(forwardVector);
		float difMag = GetMag(difVector);

		glm::vec3 unitForVec = GetUnitVector(forwardVector, forwardMag);
		glm::vec3 unitDifVec = GetUnitVector(difVector, difMag);

		glm::vec3 axisRotation = glm::cross(unitForVec, unitDifVec);

		float rotAngle = 0.f;
		if (axisRotation.y < 0)
		{
			rotAngle = 1.f;
		}
		else if (axisRotation.y > 0)
		{
			rotAngle = -1.f;
		}
		float rotationSpeed = 0.002f;

		//check to see how far the rotation went
		if (me->orientation2.y - me->orientation2.y + (rotationSpeed * rotAngle) < 0.01f || me->orientation2.y - me->orientation2.y + (rotationSpeed * rotAngle) > -0.1f)
		{
			me->diffuseColour = stateTeal;
			//move awat from the character
			glm::vec3 direction = player->position - me->position;
			direction = glm::normalize(direction);
			direction.y = 0.0f;

			if (glm::distance(player->position, me->position) <= 4.0f) {
				//performEnemyAction(player, me, direction,deltaTime);
			}
			else {
				//make the enemy move towards the player at a set speed
				//me->theEntity->move(-direction);
				me->position += -direction * this->speed * deltaTime;
				me->updateRigidBody();
			}
		}

		me->orientation2.y += (rotationSpeed * rotAngle);
	}
	else if (this->mAction == eActionType::FOLLOW) {
		glm::vec3 difVector = glm::vec3((player->position.x - me->position.x),
			(player->position.y - me->position.y),
			(player->position.z - me->position.z)
		);

		float forwardMag = GetMag(forwardVector);
		float difMag = GetMag(difVector);
		glm::vec3 unitForVec = GetUnitVector(forwardVector, forwardMag);
		glm::vec3 unitDifVec = GetUnitVector(difVector, difMag);
		glm::vec3 axisRotation = glm::cross(unitForVec, unitDifVec);

		float rotAngle = 0.f;
		if (axisRotation.y > 0)
		{
			rotAngle = 1.f;
		}
		else if (axisRotation.y < 0)
		{
			rotAngle = -1.f;
		}
		float rotationSpeed = 0.02f;

		//and to get the new forward use the rotation vector dot the
		//check to see how far the rotation went
		if (me->orientation2.y - me->orientation2.y + (rotationSpeed * rotAngle) < 0.01f || me->orientation2.y - me->orientation2.y + (rotationSpeed * rotAngle) > -0.1f)
		{
			me->diffuseColour = stateGreen;
			//move towards the character
			glm::vec3 direction = player->position - me->position;
			direction = glm::normalize(direction);

			//doesnt matter if it's a player or enemy
			if (glm::distance(me->position, player->position) < 5.0f) {
				//make sure the enemy is within the radius for a second before doing any damage
				//performEnemyAction(player, me, direction, deltaTime);
			}
			direction.y = 0.0f;
			//make the enemy move towards the player at a set speed
			me->position += direction * 5.f * deltaTime;
			//me->theEntity->move(direction);
			me->updateRigidBody();
			//for animated character
			if (me->pAnimationManager) {
				//only want one animation in here
				if (me->pAnimationManager->animationsToRun.size() == 0) {
					me->pAnimationManager->animationsToRun.push_back(me->pAnimationManager->animationStateMap["walk"].second);
				}
			}
		}
		//set the state orientation
		me->orientation2.y += (rotationSpeed * rotAngle);
	}
	else if (this->mAction == eActionType::IDLE) {
		//do nothing
		//get the distance between the player and the object
		if (me->pAnimationManager) {
			me->pAnimationManager->animationsToRun.clear();
		}

		me->diffuseColour = stateBlack;
		float dist = abs(glm::distance(me->position, player->position));
		if (dist < this->visionDistance) {
			//change my action type
			if (this->isBehindPlayer) {
				this->mAction = eActionType::FOLLOW;
			}
			else if (this->isBehindPlayer == false) {
				this->mAction = eActionType::EVADE;
			}
		}
	}
}

eStateType cFollowState::getStateType() {
	return this->mState;
}