#include "CurveYXZ.h"
#include "cGameObject.h"
#include <glm\glm.hpp>

CurveYXZ::CurveYXZ(cGameObject* theObject, bool negPos,float constValue, int iterations) {
	this->isDone = true;
	this->theObjectToMove = theObject;

	float yInitialVel = 0.0f;
	float xInitialVel = 0.0f;
	if (negPos)
	{
		for (int i = 0; i < iterations; i++)
		{
			if (i <= iterations/4)
			{
				this->thePoints.push_back(glm::vec3(xInitialVel, yInitialVel, constValue));
				yInitialVel += 0.2;
				xInitialVel += 0.1f;

			}
			else if (i > (iterations/4) && i <= (iterations/2))
			{
				this->thePoints.push_back(glm::vec3(xInitialVel, yInitialVel, constValue));
				yInitialVel -= 0.2;
				xInitialVel -= 0.1f;
			}
			else if (i > (iterations / 2) && i <= (iterations / 4 ) * 3)
			{
				this->thePoints.push_back(glm::vec3(xInitialVel, yInitialVel, constValue));
				yInitialVel -= 0.2;
				xInitialVel -= 0.1f;
			}
			else if (i >= (iterations / 4 * 3))
			{
				this->thePoints.push_back(glm::vec3(xInitialVel, yInitialVel, constValue));
				yInitialVel += 0.2;
				xInitialVel += 0.1f;
			}
		}
	}
	else
	{
		for (int i = 0; i < iterations; i++)
		{
			if (i <= iterations / 4)
			{
				this->thePoints.push_back(glm::vec3(xInitialVel, yInitialVel, constValue));
				yInitialVel -= 0.2;
				xInitialVel -= 0.1f;

			}
			else if (i > (iterations / 4) && i <= (iterations / 2))
			{
				this->thePoints.push_back(glm::vec3(xInitialVel, yInitialVel, constValue));
				yInitialVel += 0.2;
				xInitialVel += 0.1f;
			}
			else if (i > (iterations / 2) && i <= (iterations /4) *3)
			{
				this->thePoints.push_back(glm::vec3(6.0f, yInitialVel, constValue));
				yInitialVel += 0.2;
				xInitialVel += 0.1f;
			}
			else if (i >= (iterations / 4) * 3)
			{
				this->thePoints.push_back(glm::vec3(6.0f, yInitialVel, constValue));
				yInitialVel -= 0.2;
				xInitialVel -= 0.1f;
			}
		}
	}
}

CurveYXZ::~CurveYXZ() {

}

void CurveYXZ::update(double deltaTime) {

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

bool CurveYXZ::getDone() {
	return this->isDone;
}