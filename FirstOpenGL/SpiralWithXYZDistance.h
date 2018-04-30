#ifndef _SpiralWithXYZDistance_HG_
#define _SpiralWithXYZDistance_HG_

#include "iCommand.h"
#include <glm\vec3.hpp>

class cGameObject;

class SpiralWithXYZDistance : public iCommand
{
public:
	SpiralWithXYZDistance(float maxXYZ, cGameObject* theObject);
	~SpiralWithXYZDistance();

	cGameObject* theObjectToSpiral;

	virtual void update(double deltaTime);
	virtual bool getDone();

	float xDist;
	float yDist;
	float zDist;

	float spiralSize;

private:
	bool isDone;
	float maxDist;	
	float amountMoved;

	bool increase;

	float amountToMove;
};
#endif // !_SpiralWithXYZDistance_HG_
