#ifndef _CurveYXZ_HG_
#define _CurveYXZ_HG_
#include "iCommand.h"
#include <glm\vec3.hpp>
#include <vector>

class cGameObject;

class CurveYXZ :public iCommand {
public:
	CurveYXZ(cGameObject* theObject, bool negPos,float constValue, int iterations);
	~CurveYXZ();

	virtual void update(double deltaTime);
	virtual bool getDone();

	cGameObject* theObjectToMove;

private:
	int pointCounter = 0;
	std::vector<glm::vec3> thePoints;
	bool isDone;
};


#endif // !_CurveYXZ_HG_

