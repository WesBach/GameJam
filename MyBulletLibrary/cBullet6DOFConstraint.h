#ifndef _cBullet6DOFConstraint_HG_
#define _cBullet6DOFConstraint_HG_
#include <iConstraint.h>
#include <bullet\btBulletDynamicsCommon.h>
#include <glm\vec3.hpp>


namespace nPhysics {

	class cBulletRigidBody;

	//bulet 6 degrees of freedom constraint wrapper class

	class cBullet6DOFConstraint : public iConstraint {
	public:

		cBullet6DOFConstraint(cBulletRigidBody* bodyB, const glm::vec3& originB, bool useLinearReferenceFrameA);
		cBullet6DOFConstraint(cBulletRigidBody* bodyA, cBulletRigidBody* bodyB, const glm::vec3& originA, const glm::vec3& originB, bool useLinearReferenceFrameA);
		virtual ~cBullet6DOFConstraint();

		virtual iRigidBody* GetRigidBodyA();
		virtual iRigidBody* GetRigidBodyB();
		//for changing the limits of the constraint 
		virtual void SetLinearLimits(const glm::vec3& min, const glm::vec3& max);
		virtual void SetAngularLimits(const glm::vec3& min, const glm::vec3& max);

		btGeneric6DofConstraint* GetBulletConstraint();
	private:
		btGeneric6DofConstraint* m6DOF;
		cBulletRigidBody* mBodyA;
		cBulletRigidBody* mBodyB;
	};
}


#endif // !_cBullet6DOFConstraint_HG_
