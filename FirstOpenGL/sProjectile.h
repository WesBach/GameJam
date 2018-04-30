#ifndef _sProjectile_HG_
#define _sProjectile_HG_
#include <glm\vec3.hpp>
#include <iCollisionBody.h>

class cGameObject;

enum eProjectileType {
	TYPE_STANDARD,
	TYPE_EXPLOSION,
	TYPE_LONG_RANGE
};

enum eProjectileDamageType {
	DMG_NORMAL,
	DMG_EXPLOSION,
	DMG_HEAVY
};

struct sProjectile: public nPhysics::iCollisionBody {
	glm::vec3 direction;
	glm::vec3 projectileInitialPosition;
	float speed = 0.0f;
	cGameObject* object = NULL;
	int damage = -1;
	bool inUse = false;
	float projectileRange = 0.0f;
	float explosionRadius = 0.0f;
};

#endif // 

