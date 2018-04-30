#ifndef _iConstraint_HG_
#define _iConstraint_HG_
#include "enums.h"
#include"iRigidBody.h"


namespace nPhysics {

	class iConstraint {
	public:
		virtual ~iConstraint() {}
		//rigid body accessors
		virtual iRigidBody* GetRigidBodyA() = 0;
		virtual iRigidBody* GetRigidBodyB() = 0;

		//for changing the limits of the constraint 
		virtual void SetLinearLimits(const glm::vec3& min, const glm::vec3& max) {};
		virtual void SetAngularLimits(const glm::vec3& min, const glm::vec3& max) {};
		virtual void SetLinearLimits(const float& min, const float& max) {};
		virtual void SetAngularLimits(const float& min, const float& max) {};
		virtual void SetLimits(const glm::vec3& limits) {};
		virtual void SetLimits(const glm::vec3& limits, const glm::vec3& limitsRestrictions) {};
		virtual void EnableAngularMotor(bool enableMotor, float targetCelocity, float maxMotorImpulse) {};
		virtual void EnableMotor(bool enableMotor) {};
		virtual void SetMaxMotorImpulse(float maxMotorImpulse) {};
		virtual void SetMotorTargetVelocity(float motorTargetVelocity) {};
		virtual void setMotorTarget(float targetAngle, float  dt) {};

		//constraint type accessor
		inline eConstraintType GetConstraintType() const { return mConstraintType; }

	protected:
		//constructors that only the children can use
		iConstraint(eConstraintType constraintType) : mConstraintType(constraintType) {};
		iConstraint(const iConstraint& other) {};
		iConstraint() {}

		iConstraint& operator=(const iConstraint& other) { return *this; }
	private:
		eConstraintType mConstraintType;
	};
}
#endif // !_iConstraint_HG_

