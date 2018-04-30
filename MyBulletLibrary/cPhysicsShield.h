#ifndef _cPhysicsShield_HG_
#define _cPhysicsShield_HG_
#include <iCollisionBody.h>
#include <iRigidBody.h>
#include <ObjectDescriptions.h>
#include "cBulletHingeConstraint.h"


namespace nPhysics {
	class cBulletHingeConstraint;

	class cPhysicsShield : public iCollisionBody {
	public:
		cPhysicsShield(const sShieldDescription& desc);
		iRigidBody* GetBodyA();
		iRigidBody* GetBodyB();
		iRigidBody* GetBodyC();
		iRigidBody* GetShieldCenterBody();
		
		
	private:
		iRigidBody* mBodyA;
		iRigidBody* mBodyB;
		iRigidBody* mBodyC;
		//Center of the shield(all constraints will use this as the anchor point)
		iRigidBody* mShieldCenterBody;

		cBulletHingeConstraint* mHingeA;
		cBulletHingeConstraint* mHingeB;
		cBulletHingeConstraint* mHingeC;

		//create constraints for the rigid bodies internally
		void CreateMotorizedHingeConstraints();
	};
}


#endif // !_cShield_HG_

