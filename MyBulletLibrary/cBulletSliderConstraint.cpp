#include "cBulletSliderConstraint.h"
#include "cBulletRigidBody.h"
#include "nConvert.h"

namespace nPhysics {

	//constructor with a rigid 2 rigid bodies, 2 origins,and useLinearReferenceFrameA
	cBulletSliderConstraint::cBulletSliderConstraint(cBulletRigidBody* bodyA, cBulletRigidBody* bodyB, const glm::vec3& originA, const glm::vec3& originB, bool useLinearReferenceFrameA):iConstraint(eConstraintType::CONSTRAINT_TYPE_SLIDER) {
		//set the origin of a
		btTransform transA;
		transA.setIdentity();
		transA.setOrigin(nConvert::vec3ToBullet(originA));
		//set origin of b
		btTransform transB;
		transB.setIdentity();
		transB.setOrigin(nConvert::vec3ToBullet(originB));
		//create constraint
		this->mSlider = new btSliderConstraint(*bodyA->getBulletRigidBody(),*bodyB->getBulletRigidBody(),transA, transB, useLinearReferenceFrameA);
		////set the linear limits
		//mSlider->setLowerLinLimit(-15.0F);
		//mSlider->setUpperLinLimit(-5.0F);

		//set the rigid body pointers
		this->mBodyA = bodyA;
		this->mBodyB = bodyB;
	}

	//constructor with a rigid body and origin and useLinearReferenceFrameA
	cBulletSliderConstraint::cBulletSliderConstraint(cBulletRigidBody* bodyA, const glm::vec3& originA,bool useLinearReferenceFrameA) : iConstraint(eConstraintType::CONSTRAINT_TYPE_SLIDER) {
		//set rigid body origin
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(nConvert::vec3ToBullet(originA));
		//create constraint
		this->mSlider = new btSliderConstraint(*bodyA->getBulletRigidBody(), trans, useLinearReferenceFrameA);
		//set linear limits
		//mSlider->setLowerLinLimit(-15.0F);
		//mSlider->setUpperLinLimit(-5.0F);
		//set rigid body pointers
		this->mBodyA = bodyA;
		this->mBodyB = NULL;
	}

	//destructor
	cBulletSliderConstraint::~cBulletSliderConstraint()
	{
		delete this->mSlider;
		delete this->mBodyA;
		delete this->mBodyB;
	}

	//accessor for a
	iRigidBody* cBulletSliderConstraint::GetRigidBodyA() {
		return this->mBodyA;
	}

	//accessor for b
	iRigidBody* cBulletSliderConstraint::GetRigidBodyB() {
		return this->mBodyB;
	}

	//accessor for the constraint
	btSliderConstraint * cBulletSliderConstraint::GetBulletConstraint()
	{
		return this->mSlider;
	}

	//linear limit mutators
	void cBulletSliderConstraint::SetLinearLimits(const float& min, const float& max) {
		this->mSlider->setLowerLinLimit(btScalar(min));
		this->mSlider->setUpperLinLimit(btScalar(max));
	}

	//angular limit mutators
	void cBulletSliderConstraint::SetAngularLimits(const float& min, const float& max) {
		this->mSlider->setLowerAngLimit(btScalar(min));
		this->mSlider->setUpperAngLimit(btScalar(max));
	}

	void cBulletSliderConstraint::EnableAngularMotor(bool enableMotor, float targetCelocity, float maxMotorImpulse) {

	}

	void cBulletSliderConstraint::EnableMotor(bool enableMotor) {
		this->mSlider->setPoweredAngMotor(enableMotor);
	}

	void cBulletSliderConstraint::SetMaxMotorImpulse(float maxMotorImpulse) {
		this->mSlider->setMaxLinMotorForce(maxMotorImpulse);
	}

	void cBulletSliderConstraint::SetMotorTargetVelocity(float motorTargetVelocity) {
		this->mSlider->setTargetLinMotorVelocity(motorTargetVelocity);
	}

	void cBulletSliderConstraint::setMotorTarget(float targetAngle, float  dt) {

	}
}

