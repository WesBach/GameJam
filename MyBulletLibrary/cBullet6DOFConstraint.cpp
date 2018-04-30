#include "cBullet6DOFConstraint.h"
#include "cBulletRigidBody.h"
#include "nConvert.h"

namespace nPhysics {

	//Constructor with single rigid body, constraint origin and useLinearReferenceFrameA
	cBullet6DOFConstraint::cBullet6DOFConstraint(cBulletRigidBody* bodyB, const glm::vec3& originB, bool useLinearReferenceFrameA):iConstraint(eConstraintType::CONSTRAINT_TYPE_6DOF) {
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(nConvert::vec3ToBullet(originB));
		//create the constraint
		this->m6DOF = new btGeneric6DofConstraint(*bodyB->getBulletRigidBody(),trans,useLinearReferenceFrameA);
		
		//set the linear lower and upper limit(could do this somewhere else)
		m6DOF->setLinearLowerLimit(btVector3(-5., -2., -1.));
		m6DOF->setLinearUpperLimit(btVector3(5., 2., 1.));

		m6DOF->setAngularLowerLimit(btVector3(-1.5, 0, 0));
		m6DOF->setAngularUpperLimit(btVector3(1.5, 0, 0));

		m6DOF->getTranslationalLimitMotor()->m_enableMotor[0] = true;
		m6DOF->getTranslationalLimitMotor()->m_targetVelocity[0] = -5.0f;
		m6DOF->getTranslationalLimitMotor()->m_maxMotorForce[0] = 0.1f;
		//set the rigid body pointers 
		this->mBodyB = bodyB;
		this->mBodyA = NULL;
	}

	//constructor with 2 rigid bodies, origins and useLinearReferenceFrameA
	cBullet6DOFConstraint::cBullet6DOFConstraint(cBulletRigidBody* bodyA, cBulletRigidBody* bodyB, const glm::vec3& originA, const glm::vec3& originB, bool useLinearReferenceFrameA) : iConstraint(eConstraintType::CONSTRAINT_TYPE_6DOF) {
		//set origin of a
		btTransform transA;
		transA.setIdentity();
		transA.setOrigin(nConvert::vec3ToBullet(originA));
		//set the origin of b
		btTransform transB;
		transB.setIdentity();
		transB.setOrigin(nConvert::vec3ToBullet(originB));
		//create constrain
		this->m6DOF = new btGeneric6DofConstraint(*bodyA->getBulletRigidBody(),*bodyB->getBulletRigidBody(),transA,transB,useLinearReferenceFrameA);
		//set the linear upper and lower limit
		m6DOF->setLinearLowerLimit(btVector3(-10., -2., -1.));
		m6DOF->setLinearUpperLimit(btVector3(10., 2., 1.));
			
		//set the linear lower and upper limit(could do this somewhere else)
		m6DOF->setLinearLowerLimit(btVector3(-5., -2., -1.));
		m6DOF->setLinearUpperLimit(btVector3(5., 2., 1.));

		m6DOF->setAngularLowerLimit(btVector3(-1.5, 0, 0));
		m6DOF->setAngularUpperLimit(btVector3(1.5, 0, 0));

		m6DOF->getTranslationalLimitMotor()->m_enableMotor[0] = true;
		m6DOF->getTranslationalLimitMotor()->m_targetVelocity[0] = -5.0f;
		m6DOF->getTranslationalLimitMotor()->m_maxMotorForce[0] = 0.1f;

		//set the rigid body pointers
		this->mBodyA = bodyA;
		this->mBodyB = bodyB;
	}

	//destructor
	cBullet6DOFConstraint::~cBullet6DOFConstraint() {
		delete this->m6DOF;
		delete this->mBodyA;
		delete this->mBodyB;
	}
	//accessor for a
	iRigidBody* cBullet6DOFConstraint::GetRigidBodyA() {
		return this->mBodyA;
	}

	//accessor for b
	iRigidBody* cBullet6DOFConstraint::GetRigidBodyB() {
		return this->mBodyB;
	}

	//accessor for the constraint 
	btGeneric6DofConstraint * cBullet6DOFConstraint::GetBulletConstraint()
	{
		return this->m6DOF;
	}

	//linear limit mutator
	void cBullet6DOFConstraint::SetLinearLimits(const glm::vec3& min, const glm::vec3& max) {
		this->m6DOF->setLinearLowerLimit(nConvert::vec3ToBullet(min));
		this->m6DOF->setLinearUpperLimit(nConvert::vec3ToBullet(max));
	}
	//angular limit mutator
	void cBullet6DOFConstraint::SetAngularLimits(const glm::vec3& min, const glm::vec3& max) {
		this->m6DOF->setAngularLowerLimit(nConvert::vec3ToBullet(min));
		this->m6DOF->setAngularUpperLimit(nConvert::vec3ToBullet(max));
	}

}