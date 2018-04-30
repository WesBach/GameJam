#include "cBulletBallAndSocketConstraint.h"
#include "cBulletRigidBody.h"
#include "nConvert.h"

namespace nPhysics {

	//destructor
	cBulletBallAndSocketConstriant::~cBulletBallAndSocketConstriant() {
		delete this->mPointToPoint;
		delete this->mBodyA;
		delete this->mBodyB;
	}

	//constructor with a rigid body and pivot point
	cBulletBallAndSocketConstriant::cBulletBallAndSocketConstriant(cBulletRigidBody* bodyA, const glm::vec3&  pivotPoint) : iConstraint(eConstraintType::CONSTRAINT_TYPE_BALL_AND_SOCKET) {
		this->mPointToPoint = new btPoint2PointConstraint(*bodyA->getBulletRigidBody(),nConvert::vec3ToBullet(pivotPoint));
		//set the rigid body pointers
		this->mBodyA = bodyA;
		this->mBodyB = NULL;
	}

	//contstructor with 2 rigid bodies and pivot points
	cBulletBallAndSocketConstriant::cBulletBallAndSocketConstriant(cBulletRigidBody* bodyA, cBulletRigidBody* bodyB, const glm::vec3&  pivotPointA, const glm::vec3&  pivotPointB) : iConstraint(eConstraintType::CONSTRAINT_TYPE_BALL_AND_SOCKET) {
		this->mPointToPoint = new btPoint2PointConstraint(*bodyA->getBulletRigidBody(), *bodyB->getBulletRigidBody(), nConvert::vec3ToBullet(pivotPointA), nConvert::vec3ToBullet(pivotPointB));
		//set the rigid body pointers
		this->mBodyA = bodyA;
		this->mBodyB = bodyB;
	}

	//accessor for a
	iRigidBody* cBulletBallAndSocketConstriant::GetRigidBodyA() {
		return this->mBodyA;
	}
	//accessor for b
	iRigidBody* cBulletBallAndSocketConstriant::GetRigidBodyB() {
		return this->mBodyB;
	}

	//accessor for the constraint
	btPoint2PointConstraint * cBulletBallAndSocketConstriant::GetBulletConstraint()
	{
		return this->mPointToPoint;
	}

	//linear limit mutator
	void cBulletBallAndSocketConstriant::SetLinearLimits(const glm::vec3& min, const glm::vec3& max) {
	}
	//angular limit mutator
	void cBulletBallAndSocketConstriant::SetAngularLimits(const glm::vec3& min, const glm::vec3& max) {
	}
}