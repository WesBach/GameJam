#ifndef _cBulleRigidBody_HG_
#define _cBulletRigidBody_HG_
#include <iRigidBody.h>
#include <iCollisionBody.h>
#include <bullet\btBulletDynamicsCommon.h>
#include <sRigidBodyDesc.h>
#include <iShape.h>
#include <glm\gtc\quaternion.hpp>

namespace nPhysics {

//Bullet Rigid Body wrapper


class cBulletRigidBody : public iRigidBody 
{
public:
	virtual iShape* GetShape();

	cBulletRigidBody(const sRigidBodyDesc& desc, iShape* shape, eCollisionBodyType bodyType);
	virtual ~cBulletRigidBody();

	//accessor functions
	virtual void GetTransform(glm::mat4& transformOut) ;
	virtual void GetPosition(glm::vec3& positionOut);
	virtual void GetRotation(glm::vec3& rotationOut) ;
	virtual void GetPreviousPosition(glm::vec3& positionOu) ;
	virtual void GetAngularVelocity(glm::vec3& andgularVelOut);
	virtual void GetLinearVelocity(glm::vec3& andgularVelOut);
	virtual void GetTorque(glm::vec3& torqueOut) ;
	virtual void GetVelocity(glm::vec3& velOut) ;
	virtual void GetInverseMass(glm::vec3& velOut) ;
	virtual void GetMass(float& massOut) ;

	//mutator functions
	virtual void SetTransform(glm::mat4& transformIn) ;
	virtual void SetPosition(glm::vec3& positionIn) ;
	virtual void SetRotation(glm::vec3& rotationIn) ;
	virtual void SetPreviousPosition(glm::vec3& positionIn);
	virtual void SetAngularVelocity(glm::vec3& andgularVelIn) ;
	virtual void SetTorque(glm::vec3& torqueIn) ;
	virtual void SetVelocity(glm::vec3& velIn) ;
	virtual void SetInverseMass(glm::vec3& velIn) ;
	virtual void SetMass(float& massIn) ;
	virtual void SetGravity(glm::vec3& gravity);
	virtual void SetLinearVelocity(glm::vec3& velIn);
	virtual void SetLinearFactor(glm::vec3& factorIn);
	virtual void SetActivationState(bool isActive);

	virtual void ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint) ;
	virtual void applyTorque(const glm::vec3& torque);
	virtual void GetVelocityAtPoint(glm::vec3& relativePoint, glm::vec3& velocityOut) ;
	virtual void ApplyImpulse(const glm::vec3& impulse) ;

	//rigid body accessor
	btRigidBody* getBulletRigidBody() { return mBody; }
protected:
	btDefaultMotionState* mMotionState;
	btRigidBody* mBody;
	btCollisionShape* mBodyShape;
	iShape* mShape;

	friend class cBulletPhysicsWorld;
	glm::vec3 mAngularVelocity;
	glm::vec3 mInverseMass;
	glm::vec3 mTorque;

	glm::vec3 mPosition;
	glm::vec3 mVelocity;
	glm::quat mRotation;
	glm::vec3 mPreviousPosition;
	float mMass;
};

}

#endif // !_cBulleRigidBody_HG