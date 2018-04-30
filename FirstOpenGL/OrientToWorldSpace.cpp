#include "OrientToWorldSpace.h"
#include "cGameObject.h"
#include <glm\glm.hpp>

OrientToWorldSpace::OrientToWorldSpace(glm::vec3 degrees, cGameObject* theObjectToTurn, float time) {
	this->degreesToTurn = degrees;
	this->theObjectToTurn = theObjectToTurn;
	this->timeTurnTakes = time;

	this->isDone = false;
	this->sectionToDo = 0;

	this->timeInterval = time / 3;
}

OrientToWorldSpace::~OrientToWorldSpace() {

}

void OrientToWorldSpace::update(double deltaTime) {

	timeElapsed += deltaTime;
	if (sectionToDo == 0)
	{
		if (timeElapsed < timeInterval)
		{
			this->theObjectToTurn->orientation.x += glm::smoothstep(this->theObjectToTurn->orientation.x, this->degreesToTurn.x / 3, timeElapsed);
			this->theObjectToTurn->orientation.y += glm::smoothstep(this->theObjectToTurn->orientation.y, this->degreesToTurn.y / 3, timeElapsed);
			this->theObjectToTurn->orientation.z += glm::smoothstep(this->theObjectToTurn->orientation.z, this->degreesToTurn.z / 3, timeElapsed);
		}
		else
			sectionToDo = 1;
	}
	else if (sectionToDo == 1)
	{
		if (timeElapsed < (timeInterval * 2))
		{
			this->theObjectToTurn->orientation.x += glm::smoothstep(this->theObjectToTurn->orientation.x, this->degreesToTurn.x / 3, timeTurnTakes);
			this->theObjectToTurn->orientation.y += glm::smoothstep(this->theObjectToTurn->orientation.y, this->degreesToTurn.y / 3, timeTurnTakes);
			this->theObjectToTurn->orientation.z += glm::smoothstep(this->theObjectToTurn->orientation.z, this->degreesToTurn.z / 3, timeTurnTakes);
		}
		else
			sectionToDo = 2;
	}
	else if (sectionToDo == 2)
	{
		if (timeElapsed <= (timeInterval * 3))
		{
			this->theObjectToTurn->orientation.x -= glm::smoothstep(this->theObjectToTurn->orientation.x, this->degreesToTurn.x / 3, timeTurnTakes - timeElapsed);
			this->theObjectToTurn->orientation.y -= glm::smoothstep(this->theObjectToTurn->orientation.y, this->degreesToTurn.y / 3, timeTurnTakes - timeElapsed);
			this->theObjectToTurn->orientation.z -= glm::smoothstep(this->theObjectToTurn->orientation.z, this->degreesToTurn.z / 3, timeTurnTakes + timeElapsed);
		}
		else
			this->isDone = true;
	}

	//make sure it doesnt over rotate
	if (this->theObjectToTurn->orientation.x >= this->degreesToTurn.x
		&& this->theObjectToTurn->orientation.y >= this->degreesToTurn.y
		&& this->theObjectToTurn->orientation.z >= this->degreesToTurn.z
		)
	{
		this->isDone = true;
	}
}

bool OrientToWorldSpace::getDone() {
	return this->isDone;
}