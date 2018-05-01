#include "cCollisionHandler.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cSceneManager.h"
#include "cGameObject.h"
#include "sProjectile.h"
#include "cSoundManager.h"
#include "cEmitter.h"
#include <iRigidBody.h>
#include <iPhysicsFactory.h>

extern nPhysics::iPhysicsWorld* theWorld;
extern nPhysics::iPhysicsFactory* theFactory;
extern sScene* g_pCurrentScene;
extern cSoundManager* g_pSoundManager;
extern std::vector<cGameObject*>  g_vecGameObjects;
extern bool g_shakeCamera;
//Do the collision actions for each type of collision
void cCollisionHandler::Collision(nPhysics::iCollisionBody* bodyA, nPhysics::iCollisionBody* bodyB) {
	nPhysics::eCollisionBodyType bodyTypeA = bodyA->GetBodyType();
	nPhysics::eCollisionBodyType bodyTypeB = bodyB->GetBodyType();

	if (bodyTypeA == nPhysics::eCollisionBodyType::BOSS_BODY) {
		if (bodyTypeB == nPhysics::eCollisionBodyType::PLAYER_PROJECTILE_BODY) {
			if (bodyA == g_pCurrentScene->currentBoss->theBody) {
				sProjectile projectile = getProjectileFromPlayer(g_pCurrentScene->currentPlayer, bodyB);
				g_pCurrentScene->currentBoss->theEntity->takeDamage(projectile.damage);
				//remove the rigid body from the world
				g_pCurrentScene->currentPlayer->removeProjectile(projectile.object);
				g_pCurrentScene->currentPlayer->setProjectileInUse(projectile);
				//play the enemy damaged sound
				g_pSoundManager->playEnemyDamagedSound();
			}
		}
	}

	if (bodyTypeA == nPhysics::eCollisionBodyType::TREE_BODY) {
		if (bodyTypeB == nPhysics::eCollisionBodyType::PLAYER_BODY) {
			//find the tree
			for (int i = 0; i < g_pCurrentScene->terrain.size(); i++) {
				if (bodyA == g_pCurrentScene->terrain[i]->theBody) {
					//set the emitter to active
					if (g_pCurrentScene->terrain[i]->theParticleEmitter->getActive() != true) {
						g_pCurrentScene->terrain[i]->theParticleEmitter->setEmitterToActive(true);
					}
				}
			}
		}
	}

	//check for collisions between player and enemy projectile
	if (bodyTypeA == nPhysics::eCollisionBodyType::PLAYER_BODY) {
		if (bodyTypeB == nPhysics::eCollisionBodyType::ENEMY_PROJECTILE_BODY) {
			sProjectile* projectile = dynamic_cast<sProjectile*>(bodyB);
			g_pCurrentScene->currentPlayer->takeDamage(projectile->damage);
			//remove the rigid body from the world
			//removeRigidBodyFromWorld(bodyB);
			nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(bodyB);
			tempBody->SetActivationState(false);
			//play the player damaged sound
			g_pSoundManager->playPlayerDamagedSound();
		}
		else if (bodyTypeB == nPhysics::eCollisionBodyType::ITEM_HEALTH_BODY) {
			//find the power up
			for (int i = 0; i < g_pCurrentScene->scenePowerUps.size(); i++)
			{
				if (bodyB == g_pCurrentScene->scenePowerUps[i]->thePowerUp->theBody) {
					//add the health
					g_pCurrentScene->currentPlayer->currentHealth += g_pCurrentScene->scenePowerUps[i]->modifierValue;
					nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(bodyB);
					//TODO:: ask the teacher how to do this properly it doesn't stop collisions from happening
					//tempBody->SetActivationState(false);
					removeObjectFromDrawVector(bodyB, g_pCurrentScene->powerUps);
					//play the health sound
					g_pSoundManager->playHealthSound();
					theWorld->RemoveRigidBody(tempBody);
					break;
				}
			}
		}
		else if (bodyTypeB == nPhysics::eCollisionBodyType::ITEM_RANGE_BODY) {
			//find the power up
			for (int i = 0; i < g_pCurrentScene->scenePowerUps.size(); i++)
			{
				if (bodyB == g_pCurrentScene->scenePowerUps[i]->thePowerUp->theBody) {
					//add the health
					g_pCurrentScene->currentPlayer->projectileRange += g_pCurrentScene->scenePowerUps[i]->modifierValue;
					nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(bodyB);
					//tempBody->SetActivationState(false);
					removeObjectFromDrawVector(bodyB, g_pCurrentScene->powerUps);
					//play the range sound
					g_pSoundManager->playRangeSound();
					theWorld->RemoveRigidBody(tempBody);
					break;
				}
			}
		}
		else if (bodyTypeB == nPhysics::eCollisionBodyType::OBSTACLE_BODY) {
			sProjectile* projectile = dynamic_cast<sProjectile*>(bodyB);
			g_pCurrentScene->currentPlayer->takeDamage(10);
			g_pSoundManager->playPlayerDamagedSound();

			g_shakeCamera = true;
		}
		//else if (bodyTypeB == nPhysics::eCollisionBodyType::TREE_BODY) {
		//	//find the tree
		//	for (int i = 0; i < g_pCurrentScene->terrain.size(); i++) {
		//		if (bodyB == g_pCurrentScene->terrain[i]->theBody) {
		//			//populate the particle system
		//			if (g_pCurrentScene->terrain[i]->theParticleEmitter->getActive() != false) {
		//				g_pCurrentScene->terrain[i]->theParticleEmitter->position = g_pCurrentScene->terrain[i]->position;
		//				//init
		//				g_pCurrentScene->terrain[i]->theParticleEmitter->init(NUMPARTICLES, 100,
		//					glm::vec3(0.1f, 0.1f, 0.1f),	// Min init vel
		//					glm::vec3(10.1f, 10.1f, 10.1f),	// max init vel
		//					2.0f, 5.0f,
		//					glm::vec3(0.0f, 0.0f, 0.0f),
		//					glm::vec3(10.0f, 10.0f, 10.0f),
		//					glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

		//				g_pCurrentScene->terrain[i]->theParticleEmitter->setEmitterToActive(true);
		//				g_pCurrentScene->terrain[i]->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");
		//			}
		//		}
		//	}
		//}
	}

	if (bodyTypeB == nPhysics::eCollisionBodyType::PLAYER_BODY) {
		if (bodyTypeA == nPhysics::eCollisionBodyType::ENEMY_PROJECTILE_BODY) {
			sProjectile* projectile = dynamic_cast<sProjectile*>(bodyA);
			g_pCurrentScene->currentPlayer->takeDamage(projectile->damage);
			//remove the rigid body from the world
			//removeRigidBodyFromWorld(bodyA);
			nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(bodyA);
			tempBody->SetActivationState(false);
			//find the enemy that the projectile that belongs to
		}
		else if (bodyTypeA == nPhysics::eCollisionBodyType::ITEM_HEALTH_BODY) {
			//find the power up
			for (int i = 0; i < g_pCurrentScene->scenePowerUps.size(); i++)
			{
				if (bodyA == g_pCurrentScene->scenePowerUps[i]->thePowerUp->theBody) {
					//add the health
					g_pCurrentScene->currentPlayer->currentHealth += g_pCurrentScene->scenePowerUps[i]->modifierValue;
					nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(bodyA);
					tempBody->SetActivationState(false);
					removeObjectFromDrawVector(bodyA, g_pCurrentScene->powerUps);
					break;
				}
			}
		}
		else if (bodyTypeA == nPhysics::eCollisionBodyType::ITEM_RANGE_BODY) {
			//find the power up
			for (int i = 0; i < g_pCurrentScene->scenePowerUps.size(); i++)
			{
				if (bodyA == g_pCurrentScene->scenePowerUps[i]->thePowerUp->theBody) {
					//add the health
					g_pCurrentScene->currentPlayer->projectileRange += g_pCurrentScene->scenePowerUps[i]->modifierValue;
					nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(bodyA);
					tempBody->SetActivationState(false);
					removeObjectFromDrawVector(bodyA, g_pCurrentScene->powerUps);
					break;
				}
			}
		}
		else if (bodyTypeA == nPhysics::eCollisionBodyType::OBSTACLE_BODY) {
			sProjectile* projectile = dynamic_cast<sProjectile*>(bodyA);
			g_pCurrentScene->currentPlayer->takeDamage(10);
			g_pSoundManager->playPlayerDamagedSound();
			g_shakeCamera = true;
		}
	}//check to collisions between enemy and player projectile

	if (bodyTypeA == nPhysics::eCollisionBodyType::ENEMY_BODY) {
		if (bodyTypeB == nPhysics::eCollisionBodyType::PLAYER_PROJECTILE_BODY) {
			//find the enemy
			for (int i = 0; i < g_pCurrentScene->entities.size(); i++) {
				if (g_pCurrentScene->entities[i]->theBody == bodyA) {
					//make the enemy take damage
					sProjectile projectile = getProjectileFromPlayer(g_pCurrentScene->currentPlayer, bodyB);
					g_pCurrentScene->entities[i]->theEntity->takeDamage(projectile.damage);
					//remove the rigid body from the world
					g_pCurrentScene->currentPlayer->removeProjectile(projectile.object);
					g_pCurrentScene->currentPlayer->setProjectileInUse(projectile);
					//play the enemy damaged sound
					g_pSoundManager->playEnemyDamagedSound();
					break;
				}
			}
		}
	}

	if (bodyTypeB == nPhysics::eCollisionBodyType::ENEMY_BODY) {
		if (bodyTypeA == nPhysics::eCollisionBodyType::PLAYER_PROJECTILE_BODY) {
			//get the projectile
			//find the enemy
			for (int i = 0; i < g_pCurrentScene->entities.size(); i++) {
				if (g_pCurrentScene->entities[i]->theBody == bodyB) {
					//get the projectile from the player
					sProjectile projectile = getProjectileFromPlayer(g_pCurrentScene->currentPlayer, bodyA);
					g_pCurrentScene->entities[i]->theEntity->takeDamage(projectile.damage);
					//remove the rigid body from the world
					g_pCurrentScene->currentPlayer->removeProjectile(projectile.object);
					g_pCurrentScene->currentPlayer->setProjectileInUse(projectile);
					//play the enemy damaged sound
					g_pSoundManager->playEnemyDamagedSound();
					break;
				}
			}
		}
	}

	if (bodyTypeA == nPhysics::eCollisionBodyType::ITEM_HEALTH_BODY) {
		if (bodyTypeB == nPhysics::eCollisionBodyType::PLAYER_BODY) {
			//find the power up
			for (int i = 0; i < g_pCurrentScene->scenePowerUps.size(); i++)
			{
				if (bodyA == g_pCurrentScene->scenePowerUps[i]->thePowerUp->theBody) {
					//add the health
					g_pCurrentScene->currentPlayer->currentHealth += g_pCurrentScene->scenePowerUps[i]->modifierValue;
					nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(bodyA);
					//tempBody->SetActivationState(false);
					removeObjectFromDrawVector(bodyA, g_pCurrentScene->powerUps);
					//play the health sound
					g_pSoundManager->playHealthSound();
					theWorld->RemoveRigidBody(tempBody);
					break;
				}
			}
		}
	}

	if (bodyTypeA == nPhysics::eCollisionBodyType::ITEM_RANGE_BODY) {
		if (bodyTypeB == nPhysics::eCollisionBodyType::PLAYER_BODY) {
			//find the power up
			for (int i = 0; i < g_pCurrentScene->scenePowerUps.size(); i++)
			{
				if (bodyA == g_pCurrentScene->scenePowerUps[i]->thePowerUp->theBody) {
					//add the health
					g_pCurrentScene->currentPlayer->projectileRange += g_pCurrentScene->scenePowerUps[i]->modifierValue;
					nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(bodyA);
					//tempBody->SetActivationState(false);
					removeObjectFromDrawVector(bodyA, g_pCurrentScene->powerUps);
					//play the range sound
					g_pSoundManager->playRangeSound();
					theWorld->RemoveRigidBody(tempBody);
					break;
				}
			}
		}
	}
}

void cCollisionHandler::removeRigidBodyFromWorld(nPhysics::iCollisionBody* theBody) {
	//remove the projectile from the world
	nPhysics::iRigidBody* tempBod = dynamic_cast<nPhysics::iRigidBody*>(theBody);
	theWorld->RemoveRigidBody(tempBod);
}

void cCollisionHandler::setCurrentScene(sScene* currentScene) {
	//this->pCurrentScene = currentScene;
}

void cCollisionHandler::removeObjectFromDrawVector(nPhysics::iCollisionBody* body, std::vector<cGameObject*>& vectorToRemoveFrom) {
	for (std::vector<cGameObject*>::iterator it = vectorToRemoveFrom.begin(); it != vectorToRemoveFrom.end(); it++) {
		if (body == (*it)->theBody) {
			it = vectorToRemoveFrom.erase(it);
			break;
		}
	}
}

sProjectile getProjectileFromEntity(iEntity* entity, nPhysics::iCollisionBody* collisionBody) {
	eEntityType entType = entity->getEntityType();
	cPlayer* tempPlayer;
	cEnemy* tempEnemy;
	sProjectile tempProjectile;
	cGameObject* entityObject;

	//find the projectile
	switch (entType) {
	case eEntityType::ENEMY_ENTITY:
		tempEnemy = dynamic_cast<cEnemy*>(entity);
		for (int i = 0; i < tempEnemy->projectilesToDraw.size(); i++) {
			//found the game object belonging to the entity
			if (collisionBody == tempEnemy->projectilesToDraw[i]->theEntity) {
				entityObject = tempEnemy->projectilesToDraw[i];
			}
		}

		for (int i = 0; i < tempEnemy->projectiles.size(); i++) {
			if (entityObject == tempEnemy->projectiles[i].object) {
				return tempEnemy->projectiles[i];
			}
		}
		break;
	case eEntityType::PLAYER_ENTITY:
		tempPlayer = dynamic_cast<cPlayer*>(entity);
		for (int i = 0; i < tempPlayer->projectilesToDraw.size(); i++) {
			//found the game object belonging to the entity
			if (collisionBody == tempPlayer->projectilesToDraw[i]->theEntity) {
				entityObject = tempPlayer->projectilesToDraw[i];
			}
		}

		for (int i = 0; i < tempPlayer->projectiles.size(); i++) {
			if (entityObject == tempPlayer->projectiles[i].object) {
				return tempPlayer->projectiles[i];
			}
		}
		break;
	}
}

sProjectile cCollisionHandler::getProjectileFromPlayer(cPlayer* player, nPhysics::iCollisionBody* collisionBody) {
	//find the projectile
	for (int i = 0; i < player->projectiles.size(); i++) {
		if (player->projectiles[i].object->theBody == collisionBody) {
			return player->projectiles[i];
		}
	}
}