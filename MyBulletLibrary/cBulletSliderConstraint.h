#ifndef _cBulletSliderConstraint_HG_
#define _cBulletSliderConstraint_HG_
#include <glm/vec3.hpp>
#include <iConstraint.h>
#include <bullet\btBulletDynamicsCommon.h>

namespace nPhysics {
	class cBulletRigidBody;

	//Bullet Slider constraint wrapper

	class cBulletSliderConstraint : public iConstraint {
	public:
		cBulletSliderConstraint(cBulletRigidBody* bodyA, cBulletRigidBody* bodyB, const glm::vec3& originA, const glm::vec3& originB, bool useLinearReferenceFrameA);
		cBulletSliderConstraint(cBulletRigidBody* bodyB, const glm::vec3& originB, bool useLinearReferenceFrameA);
		virtual ~cBulletSliderConstraint();

		virtual iRigidBody* GetRigidBodyA();
		virtual iRigidBody* GetRigidBodyB();
		//for changing the limits of the constraint
		virtual void SetLinearLimits(const float& min, const float& max);
		virtual void SetAngularLimits(const float& min, const float& max);
		virtual void EnableAngularMotor(bool enableMotor, float targetCelocity, float maxMotorImpulse);
		virtual void EnableMotor(bool enableMotor);
		virtual void SetMaxMotorImpulse(float maxMotorImpulse);
		virtual void SetMotorTargetVelocity(float motorTargetVelocity);
		virtual void setMotorTarget(float targetAngle, float  dt);
		btSliderConstraint* GetBulletConstraint();

	private:
		btSliderConstraint* mSlider;
		cBulletRigidBody* mBodyA;
		cBulletRigidBody* mBodyB;
	};
}
#endif // ! _cBulletSliderConstraint_HG_
