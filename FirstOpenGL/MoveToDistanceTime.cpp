#include "MoveToDistanceTime.h"
#include "cGameObject.h"
#include <glm/glm.hpp>

MoveToDistanceTime::MoveToDistanceTime(glm::vec3 pointToMoveTo, cGameObject* theObjectToMove, float timeItShouldTake) {
	//set the data
	this->theObjectToMove = theObjectToMove;
	this->posToMoveTo = pointToMoveTo;

	this->isDone = false;
	this->currentSectionToProcess = 0;
	this->timeToMove = timeItShouldTake;
	this->accumulatedTime = 0.0f;

	distanceBetweenSections = pointToMoveTo;
	this->startingPosition = &theObjectToMove->position;

	if (pointToMoveTo.x > 0.0f)
		theObjectToMove->vel.x = 0.1f;
	else
		theObjectToMove->vel.x = -0.1f;

	if (pointToMoveTo.y > 0.0f)
		theObjectToMove->vel.y = 0.1f;
	else
		theObjectToMove->vel.y = -0.1f;

	if (pointToMoveTo.z > 0.0f)
		theObjectToMove->vel.z = 0.1f;
	else
		theObjectToMove->vel.z = -0.1f;

	this->initTime = 0.0;
}

MoveToDistanceTime::~MoveToDistanceTime() {

}

void MoveToDistanceTime::update(double deltaTime) {
	if (initTime == 0.0)
		initTime = deltaTime;

	accumulatedTime += deltaTime;
	float time = 0.0f;

	time = glm::smoothstep(0.0f, 1.0f, accumulatedTime / timeToMove);
	this->theObjectToMove->position = lerp(*this->startingPosition, posToMoveTo, time);

	if (accumulatedTime >= timeToMove)
		this->isDone = true;
}

bool MoveToDistanceTime::getDone() {
	return this->isDone;
}