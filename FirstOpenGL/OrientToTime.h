#ifndef  _OrientToTime_HG_
#define _OrientToTime_HG_
#include "iCommand.h"
#include <glm\vec3.hpp>

class cGameObject;

class OrientToTime : public iCommand{
public:
	OrientToTime(glm::vec3 degrees,cGameObject* theObjectToTurn, float time);
	~OrientToTime();

	virtual void update(double deltaTime);
	virtual bool getDone();

	cGameObject* theObjectToTurn;

	int sectionToDo;
private:
	glm::vec3 degreesToTurn;
	float timeTurnTakes;
	bool isDone;
	float timeElapsed;
	float timeInterval;
};
#endif // ! _OrientToTime_HG_
