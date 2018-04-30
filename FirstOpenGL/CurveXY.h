#ifndef _CurveXY_HG_
#define _CurveXY_HG_
#include "iCommand.h"
#include <glm\vec3.hpp>
#include <vector>

class cGameObject;

class CurveXY : public iCommand {
public:
	CurveXY(cGameObject* theObject);
	~CurveXY();

	virtual void update(double deltaTime);
	virtual bool getDone();

	cGameObject* theObjectToMove;

private:
	int pointCounter = 0;
	std::vector<glm::vec3> thePoints;
	bool isDone;
};
#endif // !_CurveTo_HG_

