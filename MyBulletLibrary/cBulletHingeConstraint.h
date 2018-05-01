#ifndef _cBulletHingeConstraint_HG_
#define _cBulletHingeConstraint_HG_
#include <iConstraint.h>
#include <glm\vec3.hpp>
#include <bullet\btBulletDynamicsCommon.h>

namespace nPhysics {
	class cBulletRigidBody;

	//Bullet Hinge constraint wrapper

	class cBulletHingeConstraint : public iConstraint {
	public:
		cBulletHingeConstraint(cBulletRigidBody* rbA, const glm::vec3& originA);
		cBulletHingeConstraint(cBulletRigidBody* rbA, const glm::vec3& originA, cBulletRigidBody* rbB, const glm::vec3& originB);
		cBulletHingeConstraint(cBulletRigidBody* rbA, const glm::vec3& pivot, const glm::vec3& axis);
		cBulletHingeConstraint(cBulletRigidBody* bodyA, const glm::vec3& pivot, const glm::vec3& axis, cBulletRigidBody* bodyB, const glm::vec3& pivotB, const glm::vec3& axisB);
		virtual void EnableAngularMotor(bool enableMotor, float targetCelocity, float maxMotorImpulse);
		virtual void EnableMotor(bool enableMotor);
		virtual void SetMaxMotorImpulse(float maxMotorImpulse);
		virtual void SetMotorTargetVelocity(float motorTargetVelocity);
		virtual void setMotorTarget(float targetAngle, float  dt);

		virtual ~cBulletHingeConstraint();
		virtual iRigidBody* GetRigidBodyA();
		virtual iRigidBody* GetRigidBodyB();
		btHingeConstraint* GetBulletConstraint();
		//for changing the limits of the constraint
		virtual void SetLimits(const glm::vec3& limits);
		virtual void SetLimits(const glm::vec3& limits, const glm::vec3& limitRestrictions);
	private:
		btHingeConstraint* mHinge;
		cBulletRigidBody* mBodyA;
		cBulletRigidBody* mBodyB;
	};
}
#endif // !_cBulletBallAndSocketConstraint_HG_
