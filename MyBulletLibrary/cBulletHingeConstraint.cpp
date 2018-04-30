#include "cBulletHingeConstraint.h"
#include "cBulletRigidBody.h"
#include "nConvert.h"

namespace nPhysics {

	//destructor
	cBulletHingeConstraint::~cBulletHingeConstraint() {
		delete this->mHinge;
		delete this->mBodyA;
		delete this->mBodyB;
	}

	//hinge constructor 1 rigid body and origin
	cBulletHingeConstraint::cBulletHingeConstraint(cBulletRigidBody* rbA, const glm::vec3& originA): iConstraint(eConstraintType::CONSTRAINT_TYPE_HINGE) {
		//set origin
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(nConvert::vec3ToBullet(originA));
		//create constraint
		this->mHinge = new btHingeConstraint(*rbA->getBulletRigidBody(), trans);
		//set rigig body pointers
		this->mBodyA = rbA;
		this->mBodyB = NULL;
	}

	//hinge constructor 2 rigid body 2 origin
	cBulletHingeConstraint::cBulletHingeConstraint(cBulletRigidBody* rbA, const glm::vec3& originA, cBulletRigidBody* rbB, const glm::vec3& originB) : iConstraint(eConstraintType::CONSTRAINT_TYPE_HINGE) {
		//set origin for a
		btTransform transA;
		transA.setIdentity();
		transA.setOrigin(nConvert::vec3ToBullet(originA));
		//set origin for b
		btTransform transB;
		transB.setIdentity();
		transB.setOrigin(nConvert::vec3ToBullet(originB));
		//create constraint
		this->mHinge = new btHingeConstraint(*rbA->getBulletRigidBody(),*rbB->getBulletRigidBody(),transA,transB);
		//set rigid body pointers
		this->mBodyA = rbA;
		this->mBodyB = rbB;
	}

	//hinge rigid body, pivot,axis constructor
	cBulletHingeConstraint::cBulletHingeConstraint(cBulletRigidBody* rbA, const glm::vec3& pivot, const glm::vec3& axis) : iConstraint(eConstraintType::CONSTRAINT_TYPE_HINGE) {
		this->mHinge = new btHingeConstraint(*rbA->getBulletRigidBody(), nConvert::vec3ToBullet(pivot), nConvert::vec3ToBullet(axis));
		this->mBodyA = rbA;
		this->mBodyB = NULL;
	}

	//hinge 2 rigid body,2pivot 2 axis constructor
	cBulletHingeConstraint::cBulletHingeConstraint(cBulletRigidBody* bodyA, const glm::vec3& pivot, const glm::vec3& axis, cBulletRigidBody* bodyB, const glm::vec3& pivotB, const glm::vec3& axisB) : iConstraint(eConstraintType::CONSTRAINT_TYPE_HINGE) {
		this->mHinge = new btHingeConstraint(*bodyA->getBulletRigidBody(), *bodyB->getBulletRigidBody(), nConvert::vec3ToBullet(pivot), nConvert::vec3ToBullet(pivotB), nConvert::vec3ToBullet(axis), nConvert::vec3ToBullet(axisB));
		this->mBodyA = bodyA;
		this->mBodyB = bodyB;
	}

	//accessor for a
	iRigidBody* cBulletHingeConstraint::GetRigidBodyA() {
		return this->mBodyA;
	}
	//accessor for b
	iRigidBody* cBulletHingeConstraint::GetRigidBodyB() {
		return this->mBodyB;
	}
	//accessor for the constraint
	btHingeConstraint * cBulletHingeConstraint::GetBulletConstraint()
	{
		return this->mHinge;
	}

	//set limits min
	void cBulletHingeConstraint::SetLimits(const glm::vec3& limits) {
		this->mHinge->setLimit(btScalar(limits.x), btScalar(limits.y), btScalar(limits.z));
	}

	//set limmits max
	void cBulletHingeConstraint::SetLimits(const glm::vec3& limits, const glm::vec3& limitsRestrictions) {
		this->mHinge->setLimit(btScalar(limits.x), btScalar(limits.y), btScalar(limits.z), btScalar(limitsRestrictions.x), btScalar(limitsRestrictions.y));
	}

	void cBulletHingeConstraint::EnableAngularMotor(bool enableMotor, float targetVelocity, float maxMotorImpulse) {
		this->mHinge->enableAngularMotor(enableMotor,targetVelocity,maxMotorImpulse);
	}

	void cBulletHingeConstraint::EnableMotor(bool enableMotor) {
		this->mHinge->enableMotor(enableMotor);
	}

	void cBulletHingeConstraint::SetMaxMotorImpulse(float maxMotorImpulse) {
		this->mHinge->setMaxMotorImpulse(maxMotorImpulse);
	}

	void cBulletHingeConstraint::SetMotorTargetVelocity(float motorTargetVelocity) {
		this->mHinge->setMotorTargetVelocity(motorTargetVelocity);
	}

	void cBulletHingeConstraint::setMotorTarget(float targetAngle, float  dt) {
		this->mHinge->setMotorTarget(targetAngle, dt);
	}
}