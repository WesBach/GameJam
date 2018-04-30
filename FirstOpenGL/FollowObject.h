#ifndef  _FollowObject_HG_
#define _FollowObject_HG_
#include "iCommand.h"

class cGameObject;

class FollowObject : public iCommand{
public:
	FollowObject();
	~FollowObject();

	virtual void update(double deltaTime);
	virtual bool getDone();

	cGameObject* theObject;
	cGameObject* theObjectToFollow;

private:	
	bool isDone;
};

#endif // ! _FollowObject_HG_

