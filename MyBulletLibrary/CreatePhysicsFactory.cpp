#include "CreatePhysicsFactory.h"
#include "cBulletPhysicsFactory.h"

namespace nPhysics {
	//external function to access the factory
	EXTERN_DLL_EXPORT iPhysicsFactory* CreateFactory() {
		return new cBulletPhysicsFactory();
	}
}