#pragma once
#include "iPhysicsWorld.h"
#include "iRigidBody.h"
#include "sRigidBodyDesc.h"
#include "iConstraint.h"
#include "ObjectDescriptions.h"

namespace nPhysics
{
	class iPhysicsFactory
	{
	public:
		virtual ~iPhysicsFactory() {}
		//world creation function
		virtual iPhysicsWorld* CreateWorld() = 0;
		//rigid body creation function
		virtual iRigidBody* CreateRigidBody(const sRigidBodyDesc& desc, iShape* shape, eCollisionBodyType bodyType) = 0;
		virtual iCollisionBody* CreateCollisionBody(const sRigidBodyDesc& desc, iShape* shape, eCollisionBodyType bodyType) = 0;

		//shape creation functions
		virtual iShape* CreateSphere(float radius) = 0;
		virtual iShape* CreatePlane(const glm::vec3& normal, float planeConst) = 0;
		virtual iShape* CreateBox(const glm::vec3& halfExtents) = 0;
		virtual iShape* CreateCylinder(const glm::vec3& halfExtents) = 0;
		virtual iShape* CreateCone(const float& radius, const float& height) = 0;

		//constraint creation functions
		virtual iConstraint* CreateHingeConstraint(iCollisionBody* bodyA, const glm::vec3& pivot, const glm::vec3& axis, iCollisionBody* bodyB, const glm::vec3& pivotB, const glm::vec3& axisB) = 0;
		virtual iConstraint* CreateHingeConstraint(iCollisionBody* bodyA, const glm::vec3& pivot, const glm::vec3& axis) = 0;
		virtual iConstraint* CreateHingeConstraint(iCollisionBody* bodyA, const glm::vec3& origin, iCollisionBody* bodyB, const glm::vec3& originB) = 0;
		virtual iConstraint* CreateHingeConstraint(iCollisionBody* bodyA, const glm::vec3& origin) = 0;

		virtual iConstraint* CreateBallAndSocketConstraint(iCollisionBody* bodyA, const glm::vec3& pivotPoint) = 0;
		virtual iConstraint* CreateBallAndSocketConstraint(iCollisionBody* bodyA, const glm::vec3& pivotPointA, iCollisionBody* bodyB, const glm::vec3& pivotPointB) = 0;

		virtual iConstraint* CreateSliderConstraint(iCollisionBody* bodyA, iCollisionBody* bodyB, const glm::vec3& originA, const glm::vec3& originB, bool useLinearReferenceFrameA) = 0;
		virtual iConstraint* CreateSliderConstraint(iCollisionBody* bodyB, const glm::vec3& originB, bool useLinearReferenceFrameA) = 0;

		virtual iConstraint* Create6DOFConstraint(iCollisionBody* bodyA, iCollisionBody* bodyB, const glm::vec3& originA, const glm::vec3& originB, bool useLinearReferenceFrameA) = 0;
		virtual iConstraint* Create6DOFConstraint(iCollisionBody* bodyB, const glm::vec3& originB, bool useLinearReferenceFrameA) = 0;

		virtual iConstraint* CreateConeTwistConstraint(iCollisionBody* bodyA, iCollisionBody* bodyB) = 0;
		virtual iConstraint* CreateConeTwistConstraint(iCollisionBody* bodyA) = 0;
	};
}