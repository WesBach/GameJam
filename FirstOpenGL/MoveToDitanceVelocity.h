#ifndef _MoveToDistanceVelocity_HG_
#define _MoveToDistanceVelocity_HG_
#include "iCommand.h"
#include <glm\vec3.hpp>

class cGameObject;

//Distance over time
class MoveToDistanceVelocity : public iCommand
{
public:
	MoveToDistanceVelocity(glm::vec3 pointToMoveTo, cGameObject* theObjectToMove, glm::vec3 velocity);
	~MoveToDistanceVelocity();

	virtual void update(double deltaTime);
	virtual bool getDone();

	cGameObject* theObjectToMove;

	glm::vec3 distanceBetweenSections;
	glm::vec3 posToMoveTo;

	float averageVelAccumulated;
	float averageVel;
private:
	glm::vec3 velocity;
	bool isDone;
	int currentSectionToProcess;
};

#endif // !_MoveTo_HG_

