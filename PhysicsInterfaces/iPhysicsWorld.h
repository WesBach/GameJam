#pragma once
#include "iRigidBody.h"
#include "iCollisionBody.h"
#include "iCollisionListener.h"
#include "iConstraint.h"
#include "enums.h"

namespace nPhysics
{
	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}

		virtual void TimeStep(double time, float deltaTime) = 0;
		//rigid body handling
		virtual void AddRigidBody(iRigidBody* rigidBody) = 0;
		virtual void AddCollisionBody(iCollisionBody* rigidBody) = 0;
		virtual void RemoveRigidBody(iRigidBody* rigidBody) = 0;
		//integration type
		virtual void setIntegrationType(IntegrationType theType) = 0;
		//constraint handling
		virtual void addConstraint(iConstraint* theConstraint) = 0;
		virtual void removeConstraint(iConstraint* theConstraint) = 0;
		virtual void SetCollisionListener(iCollisionListener* theListener) = 0;
	};
}