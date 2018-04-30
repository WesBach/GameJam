#ifndef _iState_HG_
#define _iState_HG_
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

enum eStateType {
	ANGRY,
	CURIOUS,
	FOLLOWER
};

enum eActionType {
	PURSUE,
	EVADE,
	ARRIVE,
	FOLLOW,
	IDLE
};

class cGameObject;

class iState {
	public:
	virtual ~iState() {}
	virtual void performAction(cGameObject* player, cGameObject* me, float deltaTime) = 0;
	virtual eStateType getStateType() = 0;
};

#endif