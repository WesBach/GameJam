#ifndef  _OrientToWorldSpace_HG_
#define _OrientToWorldSpace_HG_
#include "iCommand.h"
#include <glm\vec3.hpp>

class cGameObject;

class OrientToWorldSpace : public iCommand {
public:
	OrientToWorldSpace(glm::vec3 degrees, cGameObject* theObjectToTurn, float time);
	~OrientToWorldSpace();

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
