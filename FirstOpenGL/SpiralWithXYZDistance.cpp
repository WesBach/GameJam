#include "SpiralWithXYZDistance.h"
#include "cGameObject.h"

SpiralWithXYZDistance::SpiralWithXYZDistance(float maxXYZ,cGameObject* theObject) {
	this->maxDist = maxXYZ;
	this->isDone = false;

	increase= false;

	this->amountToMove = 0.05f;
	amountMoved = 0.0f;

	this->theObjectToSpiral = theObject;
}

SpiralWithXYZDistance::~SpiralWithXYZDistance() {

}

void SpiralWithXYZDistance::update(double deltaTime) {
	//using the max and min values of XYZ restrict the movement of the object 

	//decrease y entire time
	if (increase)
	{
		if (amountMoved >= this->maxDist)
			increase = false;

		theObjectToSpiral->position.x += amountToMove;
		theObjectToSpiral->position.y -= amountToMove;
		theObjectToSpiral->position.z += amountToMove;
		amountMoved += amountToMove;
	}
	else
	{
		if (amountMoved <= 0.0f - this->maxDist)
			increase = true;
		theObjectToSpiral->position.x -= amountToMove;
		theObjectToSpiral->position.y -= amountToMove;
		theObjectToSpiral->position.z -= amountToMove;
		amountMoved -= amountToMove;
	}
}

bool SpiralWithXYZDistance::getDone() {
	return this->isDone;
}