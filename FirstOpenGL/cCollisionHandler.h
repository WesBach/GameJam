#ifndef _cCollisionHandler_HG_
#define _cCollisionHandler_HG_
#include <iCollisionListener.h>
#include <vector>

struct sScene;
class cGameObject;
struct sProjectile;
class cPlayer;
class iEntity;

class cCollisionHandler :public nPhysics::iCollisionListener {
public:
	virtual ~cCollisionHandler() {};
	virtual void Collision(nPhysics::iCollisionBody* bodyA, nPhysics::iCollisionBody* bodyB);
	void setCurrentScene(sScene* currentScene);
	void removeRigidBodyFromWorld(nPhysics::iCollisionBody* theBody);
private:
	void removeObjectFromDrawVector(nPhysics::iCollisionBody* body, std::vector<cGameObject*>& vectorToRemoveFrom);
	sProjectile getProjectileFromEntity(iEntity* entity, nPhysics::iCollisionBody* collisionBody);
	sProjectile getProjectileFromPlayer(cPlayer* player, nPhysics::iCollisionBody* collisionBody);
	//sScene* pCurrentScene;
};

#endif // !_cCollisionHandler_HG_
