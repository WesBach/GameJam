#include "CurveXY.h"
#include "cGameObject.h"
#include <glm\glm.hpp>

CurveXY::CurveXY(cGameObject* theObject) {
	this->isDone = true;
	this->theObjectToMove = theObject;

	float initialVel = 0.0f;
	float reverseVel = 0.0f;

	for (int i = 0; i < 600; i++)
	{
		if (i <= 150)
		{
			this->thePoints.push_back(glm::vec3(6.0f, initialVel, 0.0f));
			initialVel += 0.1;
			if (i == 150)
				reverseVel -= initialVel;
		}
		else if (i > 150 && i  <= 300)
		{
			this->thePoints.push_back(glm::vec3(6.0f, initialVel, 0.0f));
			initialVel -= 0.1;
		}
		else if (i > 300 && i  <= 450)
		{
			this->thePoints.push_back(glm::vec3(6.0f, initialVel, 0.0f));
			initialVel -= 0.1;
		}
		else if (i >= 300)
		{
			this->thePoints.push_back(glm::vec3(6.0f, initialVel, 0.0f));
			initialVel += 0.1;
		}
	}

}

CurveXY::~CurveXY() {

}

void CurveXY::update(double deltaTime) {

	if (pointCounter < this->thePoints.size())
	{
		this->theObjectToMove->vel = this->thePoints[this->pointCounter];
		pointCounter++;
	}
	else
	{
		this->isDone = true;
		this->theObjectToMove->vel = glm::vec3(0.0f);
	}
	
}

bool CurveXY::getDone() {
	return this->isDone;
}