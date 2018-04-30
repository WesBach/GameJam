#ifndef _cStateManager_HG_
#define _cStateManager_HG_
#include "cGameObject.h"

class cGameObject;

class cStateManager {
public:
	void determineStateBehaviours(std::vector<cGameObject*>& objects);
};

#endif // !_cStateManager_HG_
