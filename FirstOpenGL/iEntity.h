#ifndef  _iEntity_HG_
#define _iEntity_HG_
#include <glm\vec3.hpp>
#include <iCollisionBody.h>

class cGameObject;

enum eEntityType {
	PLAYER_ENTITY = 0,
	ENEMY_ENTITY,
	BOSS_ENTITY,
	UNKNOWN_ENTITY
};

class iEntity : public nPhysics::iCollisionBody {
public:
	virtual void attack(glm::vec3 direction, float deltaTime) = 0;
	virtual void removeProjectile(cGameObject* theProjectile) = 0;
	virtual void takeDamage(int damage) = 0;
	virtual eEntityType getEntityType() = 0;
	virtual void setSpeed(float speed) = 0;
	virtual void synchronize() = 0;
	virtual void move(glm::vec3 amountToMove) = 0;
	virtual void checkProjectileBounds() = 0;
	virtual void setMaxHealth(float health) = 0;
};

#endif // ! _iEntity_HG_
