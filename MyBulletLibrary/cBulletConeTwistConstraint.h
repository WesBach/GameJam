#ifndef  _cBulletConeTwistConstraint_HG_
#define _cBulletConeTwistConstraint_HG_
#include <iConstraint.h>
#include <glm\vec3.hpp>
#include <bullet\btBulletDynamicsCommon.h>

namespace nPhysics {

	class cBulletRigidBody;

	//Bullet Cone Twist Constraint wrapper

	class cBulletConeTwistConstraint : public iConstraint {
	public:
		cBulletConeTwistConstraint(cBulletRigidBody* bodyA);
		cBulletConeTwistConstraint(cBulletRigidBody* bodyA, cBulletRigidBody* bodyB);
		virtual ~cBulletConeTwistConstraint();

		btConeTwistConstraint* GetBulletConstraint();
		virtual iRigidBody* GetRigidBodyA();
		virtual iRigidBody* GetRigidBodyB();

		virtual void SetLimits(const glm::vec3& limits);
		virtual void SetLimits(const glm::vec3& limits, const glm::vec3& limitsRestrictions);
	private:
		btConeTwistConstraint* mConeTwist;
		cBulletRigidBody* mBodyA;
		cBulletRigidBody* mBodyB;
	};
}

#endif // ! _cBulletConeTwistConstraint_HG_

