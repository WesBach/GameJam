#ifndef _cCuriousState_HG_
#define _cCuriousState_HG_
#include "iState.h"
class cGameObject;

class cCuriousState : public iState {
public:
	cCuriousState();
	virtual void performAction(cGameObject* player, cGameObject* me, float deltaTime);
	virtual eStateType getStateType();
	;

	float speed;

private:
	float visionDistance;
	float chaseThreshold;
	eStateType mState;
	eActionType mAction;
	bool isBehindPlayer;
	float timeInRadius;
};

#endif