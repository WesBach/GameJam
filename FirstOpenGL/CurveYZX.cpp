#include "CurveYZX.h"
#include "cGameObject.h"
#include <glm\glm.hpp>

CurveYZX::CurveYZX(cGameObject* theObject,bool negPos) {
	this->isDone = true;
	this->theObjectToMove = theObject;

	float yInitialVel = 0.0f;
	float zInitialVel = 0.0f;
	if (negPos)
	{
		for (int i = 0; i < 600; i++)
		{
			if (i <= 150)
			{
				this->thePoints.push_back(glm::vec3(10.0f, yInitialVel, zInitialVel));
				yInitialVel += 0.2;
				zInitialVel += 0.1f;

			}
			else if (i > 150 && i <= 300)
			{
				this->thePoints.push_back(glm::vec3(10.0f, yInitialVel, zInitialVel));
				yInitialVel -= 0.2;
				zInitialVel -= 0.1f;
			}
			else if (i > 300 && i <= 450)
			{
				this->thePoints.push_back(glm::vec3(10.0f, yInitialVel, zInitialVel));
				yInitialVel -= 0.2;
				zInitialVel -= 0.1f;
			}
			else if (i >= 300)
			{
				this->thePoints.push_back(glm::vec3(10.0f, yInitialVel, zInitialVel));
				yInitialVel += 0.2;
				zInitialVel += 0.1f;
			}
		}
	}
	else
	{
		for (int i = 0; i < 600; i++)
		{
			if (i <= 150)
			{
				this->thePoints.push_back(glm::vec3(6.0f, yInitialVel, zInitialVel));
				yInitialVel -= 0.2;
				zInitialVel -= 0.1f;

			}
			else if (i > 150 && i <= 300)
			{
				this->thePoints.push_back(glm::vec3(6.0f, yInitialVel, zInitialVel));
				yInitialVel += 0.2;
				zInitialVel += 0.1f;
			}
			else if (i > 300 && i <= 450)
			{
				this->thePoints.push_back(glm::vec3(6.0f, yInitialVel, zInitialVel));
				yInitialVel += 0.2;
				zInitialVel += 0.1f;
			}
			else if (i >= 300)
			{
				this->thePoints.push_back(glm::vec3(6.0f, yInitialVel, zInitialVel));
				yInitialVel -= 0.2;
				zInitialVel -= 0.1f;
			}
		}
	}
}

CurveYZX::~CurveYZX() {

}

void CurveYZX::update(double deltaTime) {

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

bool CurveYZX::getDone() {
	return this->isDone;
}