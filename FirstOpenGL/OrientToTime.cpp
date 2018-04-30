#include "OrientToTime.h"
#include "cGameObject.h"
#include <glm\glm.hpp>

OrientToTime::OrientToTime(glm::vec3 degrees, cGameObject* theObjectToTurn, float time) {
	this->degreesToTurn = degrees;

	this->degreesToTurn = degrees;
	this->theObjectToTurn = theObjectToTurn;
	this->timeTurnTakes = time;
	this->timeElapsed = 0.0f;

	this->isDone = false;
	this->sectionToDo = 0;

	this->timeInterval = time / 3;
}

OrientToTime::~OrientToTime() {

}

void OrientToTime::update(double deltaTime) {
	float multiplier = 0.99;
	timeElapsed += deltaTime;
	if (sectionToDo == 0)
	{
		if (timeElapsed < timeInterval)
		{
			this->theObjectToTurn->orientation2.x += glm::smoothstep(this->theObjectToTurn->orientation2.x, this->degreesToTurn.x, this->timeElapsed);
			//this->theObjectToTurn->orientation2.y += glm::smoothstep(this->theObjectToTurn->orientation2.y, this->degreesToTurn.y, this->timeElapsed);
			//this->theObjectToTurn->orientation2.z += glm::smoothstep(this->theObjectToTurn->orientation2.z, this->degreesToTurn.z, this->timeElapsed);
		}
		else
			sectionToDo = 1;
	}
	else if (sectionToDo == 1)
	{
		if (timeElapsed < (timeInterval * 2))
		{
			this->theObjectToTurn->orientation2.x += glm::smoothstep(this->theObjectToTurn->orientation2.x, this->degreesToTurn.x, this->timeElapsed);
			//this->theObjectToTurn->orientation2.y += glm::smoothstep(this->theObjectToTurn->orientation2.y, this->degreesToTurn.y, this->timeElapsed);
			//this->theObjectToTurn->orientation2.z += glm::smoothstep(this->theObjectToTurn->orientation2.z, this->degreesToTurn.z, this->timeElapsed);
		}
		else
			sectionToDo = 2;
	}
	else if (sectionToDo == 2)
	{
		if (timeElapsed <= (timeInterval * 3))
		{
			this->theObjectToTurn->orientation2.x += glm::smoothstep(this->theObjectToTurn->orientation2.x, this->degreesToTurn.x/3, this->timeElapsed);
			//this->theObjectToTurn->orientation2.y += glm::smoothstep(this->theObjectToTurn->orientation2.y, this->degreesToTurn.y, this->timeElapsed);
			//this->theObjectToTurn->orientation2.z += glm::smoothstep(this->theObjectToTurn->orientation2.z, this->degreesToTurn.z, this->timeElapsed);
		}
		else
			this->isDone = true;
	}

	//make sure it doesnt over rotate
	if (this->theObjectToTurn->orientation2.x >= this->degreesToTurn.x
		&& this->theObjectToTurn->orientation2.y >= this->degreesToTurn.y
		&& this->theObjectToTurn->orientation2.z >= this->degreesToTurn.z
		)
	{
		this->isDone = true;
	}
}

bool OrientToTime::getDone() {
	return this->isDone;
}