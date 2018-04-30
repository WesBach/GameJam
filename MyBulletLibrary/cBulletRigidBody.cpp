#include "cBulletRigidBody.h"
#include "nConvert.h"
#include "shapes.h"

namespace nPhysics {

	//accessor for the bpdy shape
	iShape* cBulletRigidBody::GetShape() {
		mBody->activate(true);
		return this->mShape;
	}

	//constructor
	cBulletRigidBody::cBulletRigidBody(const sRigidBodyDesc& desc, iShape* shape, eCollisionBodyType bodyType) {
		//create a dynamic rigidbody
		mShape = shape;

		btCollisionShape* colShape;
		float radius = 0.0f;
		shape->GetSphereRadius(radius);

		float pConst;
		glm::vec3 normal;
		shape->GetPlaneConst(pConst);
		shape->GetPlaneNormal(normal);
		//get the shape type
		eShapeType tempShapeType = shape->GetShapeType();
		//Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		//based on the shape type set collision shape 
		if (tempShapeType == eShapeType::SHAPE_TYPE_PLANE)
		{
			colShape = new btStaticPlaneShape(nConvert::vec3ToBullet(normal),btScalar(0));
			startTransform.setOrigin(btVector3(desc.Position.x, desc.Position.y, desc.Position.z));
		}
		else if(tempShapeType == eShapeType::SHAPE_TYPE_SPHERE)
		{
			colShape = new btSphereShape(btScalar(radius));
			startTransform.setOrigin(btVector3(desc.Position.x, desc.Position.y, desc.Position.z));
		}
		else if (tempShapeType == eShapeType::SHAPE_TYPE_BOX) {
			cBulletBoxShape*  tempBox = dynamic_cast<cBulletBoxShape*>(shape);
			glm::vec3 boxHalfExtents;
			tempBox->GetBoxHalfExtents(boxHalfExtents);
			colShape = new btBoxShape(nConvert::vec3ToBullet(boxHalfExtents));
			startTransform.setOrigin(btVector3(desc.Position.x, desc.Position.y, desc.Position.z));
		}
		else if (tempShapeType == eShapeType::SHAPE_TYPE_CONE) {
			cBulletConeShape*  tempCone = dynamic_cast<cBulletConeShape*>(shape);
			float radius;
			float height;
			tempCone->GetConeRadiusAndHeight(radius,height);
			colShape = new btConeShape(radius,height);
			startTransform.setOrigin(btVector3(desc.Position.x, desc.Position.y, desc.Position.z));
		}
		else {
			cBulletCylinderShape*  tempCyl = dynamic_cast<cBulletCylinderShape*>(shape);
			glm::vec3 cylinderHalfExtents;
			tempCyl->GetCylinderHalfExtents(cylinderHalfExtents);
			colShape = new btCylinderShape(nConvert::vec3ToBullet(cylinderHalfExtents));
			startTransform.setOrigin(btVector3(desc.Position.x, desc.Position.y, desc.Position.z));
		}

		//set mass
		btScalar mass(desc.Mass);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);
		//set inertia
		btVector3 localInertia(0, 0, 0);
		//set dynamic or not
		if (isDynamic) {
			colShape->calculateLocalInertia(mass, localInertia);
		}


		//set the body collision shape
		this->mBodyShape = colShape;

		//create the bullet rigid body
		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

		this->mMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, mMotionState, colShape, localInertia);
		rbInfo.m_linearDamping = 0.5f;
		rbInfo.m_angularDamping = 0.01;

		this->mBody = new btRigidBody(rbInfo);
		//if the object is kinematic make sure it doesn't deactivate
		if (desc.noCollisionResponse) {
			this->mBody->setCollisionFlags(/*this->mBody->getCollisionFlags()*/ btCollisionObject::CF_NO_CONTACT_RESPONSE);
			this->mBody->setActivationState(DISABLE_DEACTIVATION);
		}

		this->mBody->setAngularFactor(btVector3(0, 0, 0));
		this->mBody->setLinearFactor(btVector3(1, 0, 1));
		this->mBody->setGravity(btVector3(0.f,0.f,0.f));
		//call the set body type function (from iCollisionBody)
		this->SetBodyType(bodyType);
		mBody->activate(true);
		mBody->setUserPointer(this);
	}

	//transform accessor
	void cBulletRigidBody::GetTransform(glm::mat4& transformOut) {
		mBody->activate(true);
		nConvert::ToGLM(this->mBody->getWorldTransform(),transformOut);
	}

	void cBulletRigidBody::SetActivationState(bool isActive) {
		if (isActive) {
			mBody->setActivationState(DISABLE_DEACTIVATION);
		}
		else {
			//mBody->setActivationState(WANTS_DEACTIVATION);
			mBody->setCollisionFlags(64);
			mBody->activate(isActive);
		}
	}

	//postition accessor
	void cBulletRigidBody::GetPosition(glm::vec3& positionOut) {
		mBody->activate(true);
		btTransform trans;
		mBody->getMotionState()->getWorldTransform(trans);
		positionOut = nConvert::vec3ToGLM(trans.getOrigin());
	}

	//rotation accessor
	void cBulletRigidBody::GetRotation(glm::vec3& rotationOut) {
		mBody->activate(true);
		btTransform transform;
		btVector3 rot;
		//make the vec3 a quaternion
		glm::quat& rotation = glm::quat(rotationOut);
		//get the quat values
		mMotionState->getWorldTransform(transform);
		transform.getRotation().getEulerZYX(rot[2], rot[1], rot[0]);

		//nConvert::ToGLM(transform.getRotation(), rotation);
		nConvert::vec3ToGLM(rot, rotationOut);
	}

	//previous position accessor
	void cBulletRigidBody::GetPreviousPosition(glm::vec3& positionOu) {

	}

	//angular velocity accessor
	void cBulletRigidBody::GetAngularVelocity(glm::vec3& andgularVelOut) {
		mBody->activate(true);
	}

	//torque accessor
	void cBulletRigidBody::GetTorque(glm::vec3& torqueOut) {

	}
	//velocity accessor
	void cBulletRigidBody::GetVelocity(glm::vec3& velOut) {
		mBody->activate(true);
		velOut = nConvert::vec3ToGLM(mBody->getLinearVelocity());
	}
	
	//inverse mass accessor
	void cBulletRigidBody::GetInverseMass(glm::vec3& velOut) {

	}
	//mass accessor
	void cBulletRigidBody::GetMass(float& massOut) {

	}
	//transform mutator
	void cBulletRigidBody::SetTransform(glm::mat4& transformIn) {
		
	}
	//position mutator
	void cBulletRigidBody::SetPosition(glm::vec3& positionIn) {
		//TODO::set the rigid body position
		btTransform transform;
		mMotionState->getWorldTransform(transform);
		transform.setOrigin(nConvert::vec3ToBullet(positionIn));
		mBody->setWorldTransform(transform);
		mMotionState->setWorldTransform(transform);
	}
	//previous position mutator
	void cBulletRigidBody::SetPreviousPosition(glm::vec3& positionIn) {

	}
	//angular vel mutator
	void cBulletRigidBody::SetAngularVelocity(glm::vec3& andgularVelIn) {
		mBody->activate(true);
		btVector3 tempVec =	nConvert::vec3ToBullet(andgularVelIn);
		mBody->setAngularVelocity(tempVec);
	}
	//torque mutator
	void cBulletRigidBody::SetTorque(glm::vec3& torqueIn) {
		
	}
	//vel mutator
	void cBulletRigidBody::SetVelocity(glm::vec3& velIn) {
		mBody->activate(true);
		btVector3 imp = nConvert::vec3ToBullet(velIn);
		this->mBody->applyCentralForce(imp);
	}
	//inv mass mutator
	void cBulletRigidBody::SetInverseMass(glm::vec3& velIn) {

	}
	//mass mutator
	void cBulletRigidBody::SetMass(float& massIn) {

	}
	//impulse application
	void cBulletRigidBody::ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint) {
		mBody->activate(true);
		btVector3 imp = nConvert::vec3ToBullet(impulse);
		btVector3 relPoint = nConvert::vec3ToBullet(relativePoint);
		mBody->applyImpulse(imp, relPoint);
	}
	//torque application
	void cBulletRigidBody::applyTorque(const glm::vec3& torque) {
		mBody->activate(true);
	}
	//point velocity accessor
	void cBulletRigidBody::GetVelocityAtPoint(glm::vec3& relativePoint, glm::vec3& velocityOut) {
		mBody->activate(true);
		btVector3 point = nConvert::vec3ToBullet(relativePoint);
		btVector3 vel = mBody->getVelocityInLocalPoint(point);
		velocityOut = nConvert::vec3ToGLM(vel);
	}
	//impule application
	void cBulletRigidBody::ApplyImpulse(const glm::vec3& impulse) {
		mBody->activate(true);
		btVector3 imp = nConvert::vec3ToBullet(impulse);
		mBody->applyCentralImpulse(imp);
	}
	//destructor
	cBulletRigidBody::~cBulletRigidBody() {
		delete this->mMotionState;
		delete this->mBody;
		delete this->mShape;
		mShape = 0;
	}
	//rotation mutator
	void cBulletRigidBody::SetRotation(glm::vec3& rotationIn) {
		mBody->activate(true);
		btTransform transform;
		//convert vec3 to quat
		glm::quat rotation(rotationIn);
		mMotionState->getWorldTransform(transform);
		//set the rotation
		transform.setRotation(nConvert::ToBullet(rotation));
		mMotionState->setWorldTransform(transform);
		mBody->setWorldTransform(transform);
	}

	void cBulletRigidBody::SetLinearVelocity(glm::vec3& velIn) {
		btRigidBody* tempBody = this->getBulletRigidBody();
		tempBody->setLinearVelocity(nConvert::vec3ToBullet(velIn));
	}

	void cBulletRigidBody::SetGravity(glm::vec3& gravity){
		btRigidBody* tempBody = this->getBulletRigidBody();
		tempBody->setGravity(nConvert::vec3ToBullet(gravity));
	}

	void cBulletRigidBody::SetLinearFactor(glm::vec3& factorIn) {
		btRigidBody* tempBody = this->getBulletRigidBody();
		tempBody->setLinearVelocity(nConvert::vec3ToBullet(factorIn));
	}

	void cBulletRigidBody::GetLinearVelocity(glm::vec3& linearVelOut) {
		btRigidBody* tempBody = this->getBulletRigidBody();
		linearVelOut = nConvert::vec3ToGLM(tempBody->getLinearVelocity());
	}


}
