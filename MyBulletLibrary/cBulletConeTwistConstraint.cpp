#include "cBulletConeTwistConstraint.h"
#include "cBulletRigidBody.h"
#include "nConvert.h"

namespace nPhysics {

	//construct with one rigid body
	cBulletConeTwistConstraint::cBulletConeTwistConstraint(cBulletRigidBody* bodyA):iConstraint(eConstraintType::CONSTRAINT_TYPE_CONE_TWIST) {
		//new btConeTwistConstraint(*pBodyA, *pBodyB, frameInA, frameInB);
		glm::vec3 origin;
		bodyA->GetPosition(origin);
		btTransform frameInA;
		frameInA.setIdentity();		
		frameInA.setOrigin(nConvert::vec3ToBullet(origin));

		frameInA = btTransform::getIdentity();
		frameInA.getBasis().setEulerZYX(0, 0, 0.2);
		frameInA.setOrigin(btVector3(btScalar(origin.x), btScalar(-5.), btScalar(origin.z)));

		this->mConeTwist = new btConeTwistConstraint(*bodyA->getBulletRigidBody(), frameInA);
		this->mBodyB = NULL;
		this->mBodyA = bodyA;
	}

	//construct the cone twist with 2 ridig bodies
	cBulletConeTwistConstraint::cBulletConeTwistConstraint(cBulletRigidBody* bodyA, cBulletRigidBody* bodyB) :iConstraint(eConstraintType::CONSTRAINT_TYPE_CONE_TWIST) {
		glm::vec3 originA;
		bodyA->GetPosition(originA);
		glm::vec3 originB;
		bodyB->GetPosition(originB);

		btTransform frameInA;
		frameInA.setIdentity();
		frameInA.setOrigin(nConvert::vec3ToBullet(originA));

		btTransform frameInB;
		frameInB.setIdentity();
		frameInB.setOrigin(nConvert::vec3ToBullet(originB));

		this->mConeTwist = new btConeTwistConstraint(*bodyA->getBulletRigidBody(), *bodyB->getBulletRigidBody(), frameInA, frameInB);

		this->mBodyA = bodyA;
		this->mBodyB = bodyB;
	}

	//destructor
	cBulletConeTwistConstraint::~cBulletConeTwistConstraint() {
		delete this->mConeTwist;
		delete this->mBodyA;
		delete this->mBodyB;
	}

	//get constraint
	btConeTwistConstraint* cBulletConeTwistConstraint::GetBulletConstraint() {
		return this->mConeTwist;
	}

	//accessor for b
	iRigidBody* cBulletConeTwistConstraint::GetRigidBodyA() {
		return this->mBodyA;
	}

	//accessor for b
	iRigidBody* cBulletConeTwistConstraint::GetRigidBodyB() {
		return this->mBodyB;
	}

	//set the cone limits
	void cBulletConeTwistConstraint::SetLimits(const glm::vec3& limits) {
		//swing,twist,softness
		this->mConeTwist->setLimit(btScalar(limits.x), btScalar(limits.y), btScalar(limits.z));
	}

	void cBulletConeTwistConstraint::SetLimits(const glm::vec3& limits , const glm::vec3& limitsRestrictions) {
		//swing span,twist span,softness,bias factor,relaxation factor
		this->mConeTwist->setLimit(btScalar(limits.x), btScalar(limits.y), btScalar(limits.z),btScalar(limitsRestrictions.x),btScalar(limitsRestrictions.y));
	}

}

