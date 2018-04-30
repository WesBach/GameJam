#ifndef _cFollowState_HG_
#define _cFollowState_HG_
#include "iState.h"
class cGameObject;

class cFollowState : public iState {
public:
	cFollowState();
	virtual void performAction(cGameObject* player, cGameObject* me, float deltaTime);
	virtual eStateType getStateType();
	float speed;

private:
	float visionDistance;
	float chaseThreshold;
	eStateType mState;
	eActionType mAction;
	bool isBehindPlayer;
	float timeInRadius;
};

#endif // !_cState_HG_
