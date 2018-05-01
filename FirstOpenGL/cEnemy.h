#ifndef _cEnemy_HG_
#define _cEnemy_HG_
#include "iEntity.h"
#include "sProjectile.h"
#include <vector>

enum eEnemyType {
	GUNNER,
	SUICIDE
};

enum eAttackType {
	PROJECTILE,
	EXPLOSION
};

class cEnemy : public iEntity {
public:
	cEnemy();
	~cEnemy();
	//for attacking
	eEnemyType enemyType;
	eAttackType attackType;
	glm::vec3 direction;
	int particleManagerEmitterIndex;

	float projectileRange;
	float health;
	float maxHealth;
	float speed;

	bool isExploding;
	cGameObject* theEnemyObject;

	std::vector<cGameObject*> projectilesToDraw;
	std::vector<sProjectile> projectilePool;
	std::vector<sProjectile> projectiles;
	//vector for sake of not writing another render function
	std::vector<cGameObject*> explosion;

	float timeElapsedBetweenProjectiles;
	float explosionSize;

	virtual void attack(glm::vec3 direction, float deltaTime);
	virtual void removeProjectile(cGameObject* theProjectile);
	virtual void takeDamage(int damage);
	virtual eEntityType getEntityType();
	virtual void setSpeed(float speed);
	virtual void synchronize();
	virtual void move(glm::vec3 amountToMove);
	virtual void checkProjectileBounds();

	void setMaxHealth(float health);

	void removeExplosion(cGameObject* theExplosion);
	sProjectile createProjectileBasedOnEnemyStats();
	int getNextProjectileIndex();
};

#endif // !_Enemy_HG_
