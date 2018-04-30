#ifndef _cBulletPhysicsFactory_HG_
#define _cBulletPhysicsFactory_HG_
#include <iPhysicsFactory.h>
#include <iConstraint.h>
#include <ObjectDescriptions.h>

namespace nPhysics {
	class cBulletPhysicsFactory : public iPhysicsFactory{
	public:
		virtual ~cBulletPhysicsFactory();
		cBulletPhysicsFactory();
		virtual iPhysicsWorld* CreateWorld();

		//create rigid body
		virtual iRigidBody* CreateRigidBody(const sRigidBodyDesc& desc, iShape* shape, eCollisionBodyType bodyType);
		virtual iCollisionBody* CreateCollisionBody(const sRigidBodyDesc& desc, iShape* shape, eCollisionBodyType bodyType);

		//Functions to create shapes for the rigid body
		virtual iShape* CreateSphere(float radius);
		virtual iShape* CreatePlane(const glm::vec3& normal, float planeConst);
		virtual iShape* CreateCylinder(const glm::vec3& halfExtents);
		virtual iShape* CreateBox(const glm::vec3& halfExtents);
		virtual iShape* CreateCone(const float& radius, const float& height);


		//Functions to create constraints for the rigid bodies
		virtual iConstraint* CreateHingeConstraint(iCollisionBody* bodyA, const glm::vec3& pivot, const glm::vec3& axis, iCollisionBody* bodyB, const glm::vec3& pivotB, const glm::vec3& axisB);
		virtual iConstraint* CreateHingeConstraint(iCollisionBody* bodyA, const glm::vec3& pivot, const glm::vec3& axis);
		virtual iConstraint* CreateHingeConstraint(iCollisionBody* bodyA, const glm::vec3& origin, iCollisionBody* bodyB, const glm::vec3& originB);
		virtual iConstraint* CreateHingeConstraint(iCollisionBody* bodyA, const glm::vec3& origin);

		virtual iConstraint* CreateBallAndSocketConstraint(iCollisionBody* bodyA,const glm::vec3& pivotPoint);
		virtual iConstraint* CreateBallAndSocketConstraint(iCollisionBody* bodyA, const glm::vec3& pivotPointA, iCollisionBody* bodyB, const glm::vec3& pivotPointB);
		
		virtual iConstraint* CreateSliderConstraint(iCollisionBody* bodyA, iCollisionBody* bodyB, const glm::vec3& originA, const glm::vec3& originB, bool useLinearReferenceFrameA);
		virtual iConstraint* CreateSliderConstraint(iCollisionBody* bodyB, const glm::vec3& originB, bool useLinearReferenceFrameA);

		virtual iConstraint* Create6DOFConstraint(iCollisionBody* bodyA, iCollisionBody* bodyB,const glm::vec3& originA,const glm::vec3& originB,bool useLinearReferenceFrameA );
		virtual iConstraint* Create6DOFConstraint(iCollisionBody* bodyB, const glm::vec3& originB, bool useLinearReferenceFrameA);

		virtual iConstraint* CreateConeTwistConstraint(iCollisionBody* bodyA, iCollisionBody* bodyB);
		virtual iConstraint* CreateConeTwistConstraint(iCollisionBody* bodyA);

	};
}

#endif // !_cBulletPhysicsFactory_HG_

