#ifndef _CurveYZX_HG_
#define _CurveYZX_HG_
#include "iCommand.h"
#include <glm\vec3.hpp>
#include <vector>

class cGameObject;

class CurveYZX : public iCommand {
public:
	CurveYZX(cGameObject* theObject,bool negPos);
	~CurveYZX();

	virtual void update(double deltaTime);
	virtual bool getDone();

	cGameObject* theObjectToMove;

private:
	int pointCounter = 0;
	std::vector<glm::vec3> thePoints;
	bool isDone;
};
#endif // !_CurveXZY_HG_
