#ifndef _cBulletPhysicsWorld_HG_
#define _cBulletPhysicsWorld_HG_
#include <iPhysicsWorld.h>
#include <bullet\btBulletDynamicsCommon.h>
#include <iCollisionListener.h>

namespace nPhysics {

	//Bullet Physics world wrapper

	class cBulletPhysicsWorld : public iPhysicsWorld {
	public:
		cBulletPhysicsWorld();
		~cBulletPhysicsWorld();

		void TimeStep(double time, float deltaTime);
		void AddRigidBody(iRigidBody* rigidBody);
		void AddCollisionBody(iCollisionBody* collisionBody);
		void RemoveRigidBody(iRigidBody* rigidBody);
		void setIntegrationType(IntegrationType theType);
		void addConstraint(iConstraint* theConstraint);
		void removeConstraint(iConstraint* theConstraint);
		bool checkIfRigidBodyExistsInWorld(iRigidBody* rigidBody);
		void SetCollisionListener(iCollisionListener* theListener);

	protected:
		void updateCollisionListener();
		btDefaultCollisionConfiguration* mCollisionConfiguration;// = new btDefaultCollisionConfiguration();
		//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* mDispatcher; //= new btCollisionDispatcher(collisionConfiguration);
		//btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		btBroadphaseInterface* mOverlappingPairCache;// = new btDbvtBroadphase();
		//the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* mSolver;// = new btSequentialImpulseConstraintSolver;
		btDiscreteDynamicsWorld* mDynamicsWorld;// = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
		iCollisionListener*  mCollisionListener;
	};
}

#endif // !_cBulletPhysicsFactory_HG_
