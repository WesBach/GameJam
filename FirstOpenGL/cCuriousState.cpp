#include "cCuriousState.h"
#include "cGameObject.h"
#include <glm\glm.hpp>
#include "cMathHelper.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cSceneManager.h"
#include "iEntity.h"

extern sScene* g_pCurrentScene;

cCuriousState::cCuriousState() {
	this->mAction = eActionType::IDLE;
	this->mState = eStateType::CURIOUS;
	this->visionDistance = 7.0f;
	this->chaseThreshold = 14.0f;
	this->speed = 0.0f;
	this->timeInRadius = 0.0f;
}

const glm::vec4 stateGrey(0.5f, 0.5f, 0.5f, 1.0f);
const glm::vec4 stateBlue(0.0f, 0.0f, 1.0f, 1.0f);
const glm::vec4 statePurple(1.0f, 0.0f, 1.0f, 1.0f);

void cCuriousState::performAction(cGameObject* player, cGameObject* me, float deltaTime) {
	float enemySpeed = 2.0f;
	glm::mat4 rotationMatrix = IDENTITYMATRIX;
	Matrix RotationMatrix = IDENTITY_MATRIX;

	RotateAboutY(&RotationMatrix, me->orientation2.y);
	glm::vec3 forwardVector = glm::vec3(1.0f, 0.0f, 0.0f);
	forwardVector = VectorMultiplyMatrices(&RotationMatrix, &forwardVector);

	//RotationMatrix = IDENTITY_MATRIX;
	//RotateAboutY(&RotationMatrix, player->orientation2.y);
	//glm::vec3 playerforwardVector = glm::vec3(1.0f, 0.0f, 0.0f);
	//playerforwardVector = VectorMultiplyMatrices(&RotationMatrix, &playerforwardVector);

	////set the players forward vector
	//g_pCurrentScene->currentPlayer->playerForward = playerforwardVector;

	float playerLookingAtMe = glm::dot(forwardVector, g_pCurrentScene->currentPlayer->playerForward);

	//set evade or arrive based on where the player is looking
	if (playerLookingAtMe < 0.0f) {
		this->mAction = eActionType::EVADE;
		this->isBehindPlayer = false;
	}
	else if (playerLookingAtMe > 0.0f) {
		this->mAction = eActionType::ARRIVE;
		this->isBehindPlayer = true;
	}

	//if the player is outside of the vision then idle
	if (abs(glm::distance(me->position, player->position)) >= chaseThreshold) {
		this->mAction = eActionType::IDLE;
	}

	if (this->mAction == eActionType::ARRIVE) {
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
		float rotationSpeed = 0.01f;

		//check to see how far the rotation went
		if (me->orientation2.y - me->orientation2.y + (rotationSpeed * rotAngle) < 0.01f || me->orientation2.y - me->orientation2.y + (rotationSpeed * rotAngle) > -0.1f)
		{
			//move towards the character
			me->diffuseColour = stateBlue;
			glm::vec3 direction = player->position - me->position;
			direction = glm::normalize(direction);
			direction.y = 0.0f;

			//performEnemyAction(player, me, direction, deltaTime);

			//make the enemy move towards the player at a set speed
			me->position += direction * this->speed * deltaTime;
			me->updateRigidBody();

			//me->theEntity->move(direction);
		}

		me->orientation2.y += (rotationSpeed * rotAngle);
	}
	else if (this->mAction == eActionType::EVADE) {
		//turn away from the person and run
		//TODO:: figure out how to make the enemy run away
		glm::vec3 difVector = glm::vec3((player->position.x - me->position.x),
			(player->position.y - me->position.y),
			(player->position.z - me->position.z)
		);

		forwardVector = -forwardVector;

		float forwardMag = GetMag(forwardVector);
		float difMag = GetMag(difVector);

		glm::vec3 unitForVec = GetUnitVector(forwardVector, forwardMag);
		glm::vec3 unitDifVec = GetUnitVector(difVector, difMag);
		//determing the rotation
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
			me->diffuseColour = statePurple;
			//move awat from the character
			glm::vec3 direction = player->position - me->position;
			direction = glm::normalize(direction);
			direction.y = 0.0f;

			//performEnemyAction(player,me, direction,deltaTime);
			//make the enemy move towards the player at a set speed
			me->position += -direction * this->speed * deltaTime;
			me->updateRigidBody();
			//me->theEntity->move(-direction);
		}
		me->orientation2.y += (rotationSpeed * rotAngle);
	}

	else if (this->mAction == eActionType::IDLE) {
		//do nothing
		//get the distance between the player and the object
		me->diffuseColour = stateGrey;
		float dist = glm::distance(player->position, me->position);
		if (dist < this->visionDistance) {
			//change my action type
			if (this->isBehindPlayer) {
				this->mAction = eActionType::ARRIVE;
			}
			else {
				this->mAction = eActionType::EVADE;
			}
		}
	}
}

eStateType cCuriousState::getStateType() {
	return this->mState;
}