#include "MoveToDitanceVelocity.h"
#include "cGameObject.h"
#include <glm\glm.hpp>

MoveToDistanceVelocity::MoveToDistanceVelocity(glm::vec3 pointToMoveTo, cGameObject* theObjectToMove, glm::vec3 velocity) {
	//set the data
	this->theObjectToMove = theObjectToMove;
	this->posToMoveTo = pointToMoveTo;

	this->isDone = false;
	this->currentSectionToProcess = 0;
	this->velocity = velocity;

	//get the average velocity to keep track of
	averageVel = abs(velocity.x) + abs(velocity.y) + abs(velocity.z) / 3;

	//distance to keep track of
	distanceBetweenSections.x = pointToMoveTo.x / 3;
	distanceBetweenSections.y = pointToMoveTo.y / 3;
	distanceBetweenSections.z = pointToMoveTo.z / 3;

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
}

MoveToDistanceVelocity::~MoveToDistanceVelocity() {

}

void MoveToDistanceVelocity::update(double deltaTime) {

	if (currentSectionToProcess == 0)
	{
		if (averageVelAccumulated >= averageVel / 3)
			currentSectionToProcess = 1;
		else {
			if (distanceBetweenSections.x > 0.0f)
				this->theObjectToMove->vel.x += glm::smoothstep(this->theObjectToMove->position.x, this->theObjectToMove->position.x + distanceBetweenSections.x, averageVelAccumulated +averageVel );
			else if (distanceBetweenSections.x < 0.0f)
				this->theObjectToMove->vel.x -= glm::smoothstep(abs(this->theObjectToMove->position.x), abs(this->theObjectToMove->position.x + distanceBetweenSections.x), averageVelAccumulated + averageVel);

			if (distanceBetweenSections.y > 0.0f)
				this->theObjectToMove->vel.y += glm::smoothstep(this->theObjectToMove->position.y, this->theObjectToMove->position.y + distanceBetweenSections.y, averageVelAccumulated + averageVel);
			else if (distanceBetweenSections.y < 0.0f)
				this->theObjectToMove->vel.y -= glm::smoothstep(abs(this->theObjectToMove->position.y), abs(this->theObjectToMove->position.y + distanceBetweenSections.y), averageVelAccumulated + averageVel);

			if (distanceBetweenSections.z >0.0f)
				this->theObjectToMove->vel.z += glm::smoothstep(this->theObjectToMove->position.z, this->theObjectToMove->position.z + distanceBetweenSections.z, averageVelAccumulated + averageVel);
			else if (distanceBetweenSections.z < 0.0f)
				this->theObjectToMove->vel.z -= glm::smoothstep(abs(this->theObjectToMove->position.z), abs(this->theObjectToMove->position.z + distanceBetweenSections.z), averageVelAccumulated + averageVel);
		
			averageVelAccumulated += abs(theObjectToMove->vel.x) + abs(theObjectToMove->vel.y) + abs(theObjectToMove->vel.z) / 3;
		}
	}

	if (currentSectionToProcess == 1)
	{
		if (averageVelAccumulated >= averageVel / 3 * 2)
			currentSectionToProcess = 2;
	}

	if (currentSectionToProcess == 2)
	{
		if (averageVelAccumulated >= averageVel)
			this->isDone = true;
		else {

			if (distanceBetweenSections.x > 0.0f)
				this->theObjectToMove->vel.x -= glm::smoothstep(this->theObjectToMove->position.x, this->theObjectToMove->position.x + distanceBetweenSections.x, averageVel - averageVelAccumulated);
			else if (distanceBetweenSections.x < 0.0f)
				this->theObjectToMove->vel.x += glm::smoothstep(this->theObjectToMove->position.x, this->theObjectToMove->position.x + distanceBetweenSections.x, averageVel - averageVelAccumulated);

			if (distanceBetweenSections.y > 0.0f)
				this->theObjectToMove->vel.y -= glm::smoothstep(this->theObjectToMove->position.y, this->theObjectToMove->position.y + distanceBetweenSections.y, averageVel - averageVelAccumulated);
			else if (distanceBetweenSections.y < 0.0f)
				this->theObjectToMove->vel.y += glm::smoothstep(this->theObjectToMove->position.y, this->theObjectToMove->position.y + distanceBetweenSections.y, averageVel - averageVelAccumulated);

			if (distanceBetweenSections.z >0.0f)
				this->theObjectToMove->vel.z -= glm::smoothstep(this->theObjectToMove->position.z, this->theObjectToMove->position.z + distanceBetweenSections.z, averageVel - averageVelAccumulated);
			else if (distanceBetweenSections.z < 0.0f)
				this->theObjectToMove->vel.z += glm::smoothstep(this->theObjectToMove->position.z, this->theObjectToMove->position.z + distanceBetweenSections.z, averageVel - averageVelAccumulated);
		}
	}
}

bool MoveToDistanceVelocity::getDone() {
	return this->isDone;
}