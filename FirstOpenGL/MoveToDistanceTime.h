#ifndef _MoveToDistanceTime_HG_
#define _MoveToDistanceTime_HG_
#include "iCommand.h"
#include <glm\vec3.hpp>

class cGameObject;

//Distance over time
class MoveToDistanceTime : public iCommand
{
public:
	MoveToDistanceTime(glm::vec3 pointToMoveTo, cGameObject* theObjectToMove, float timeItShouldTake);
	~MoveToDistanceTime();

	virtual void update(double deltaTime);
	virtual bool getDone();

	cGameObject* theObjectToMove;
	glm::vec3 distanceBetweenSections;
	glm::vec3 posToMoveTo;

	glm::vec3 fullDistance;
	glm::vec3 endDist1Point;
	glm::vec3 endDist2Point;
	glm::vec3* startingPosition;

	float timeToMove;
	float accumulatedTime;


	float lerp(float v0, float v1, float t) {
		return (1 - t) * v0 + t * v1;
	}

	glm::vec3 lerp(glm::vec3 vec1, glm::vec3 vec2, float time)
	{
		glm::vec3 temp(0.0f);

		temp.x = lerp(vec1.x, vec2.x, time);
		temp.y = lerp(vec1.y, vec2.y, time);
		temp.z = lerp(vec1.z, vec2.z, time);

		return temp;
	}

private:
	bool isDone;
	int currentSectionToProcess;
	double initTime;
};

#endif // !_MoveTo_HG_

