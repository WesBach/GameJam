#ifndef _cBulletBallAndSocketConstriant_HG_
#define _cBulletBallAndSocketConstriant_HG_
#include <iConstraint.h>
#include <bullet\btBulletDynamicsCommon.h>
#include <glm\vec3.hpp>


namespace nPhysics {

	class cBulletRigidBody;

	//Bullet Point to Point Constraint wrapper class

	class cBulletBallAndSocketConstriant : public iConstraint{
	public:
		cBulletBallAndSocketConstriant(cBulletRigidBody* bodyA,const glm::vec3& pivotPoint);
		cBulletBallAndSocketConstriant(cBulletRigidBody* bodyA, cBulletRigidBody* bodyB, const glm::vec3& pivotPointA, const glm::vec3&  pivotPointB);
		virtual ~cBulletBallAndSocketConstriant();

		iRigidBody* GetRigidBodyA();
		iRigidBody* GetRigidBodyB();
		btPoint2PointConstraint* GetBulletConstraint();

		void cBulletBallAndSocketConstriant::SetLinearLimits(const glm::vec3& min, const glm::vec3& max);
		void cBulletBallAndSocketConstriant::SetAngularLimits(const glm::vec3& min, const glm::vec3& max);
	private:
		btPoint2PointConstraint* mPointToPoint;
		cBulletRigidBody* mBodyA;
		cBulletRigidBody* mBodyB;
	};
}
#endif // 
