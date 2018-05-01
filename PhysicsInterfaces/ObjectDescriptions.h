#ifndef _ObjectDescriptions_HG_
#define _ObjectDescriptions_HG_
#include "iRigidBody.h"
#include <vector>
namespace nPhysics {
	struct sShieldDescription {
		//3 rigid bodies
		iRigidBody* bodyA;
		iRigidBody* bodyB;
		iRigidBody* bodyC;
		//anchor point for all other bodies
		iRigidBody* shieldCenterBody;
	};

	struct sEntityDescription {
		iRigidBody* entityBody;
		bool isPlayer;
		float health;
	};

	struct sBossDescription {
		std::vector<iRigidBody*> boneBodies;
		float health;
	};
}

#endif // !_ObjectDescriptions_HG_
