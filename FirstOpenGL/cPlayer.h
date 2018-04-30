#ifndef _cPlayer_HG_
#define _cPlayer_HG_
#include "iEntity.h"
#include <vector>
#include "sProjectile.h"
class cEmitter;

class cPlayer: public iEntity {
public:
	cPlayer();
	~cPlayer();
	cGameObject* thePlayerObject;
	//for shooting
	glm::vec3 playerForward;
	bool isInExplosionRadius;
	unsigned int currentHealth;
	unsigned int maxHealth;
	int maxProjectilesAllowed;
	int projectileIndex;
	float timeInExplosion;
	float playerSpeed;
	float rotationSpeed;
	float projectileRange;

	void move(glm::vec3 amountToMove);
	void rotatePlayer(glm::vec3 rotationDirection);
	void createProjectiles();
	//inherited functions
	void applyContinuousDirectionalVelocityToProjectiles();
	virtual void attack(glm::vec3 direction, float deltaTime);
	virtual void removeProjectile(cGameObject* theProjectile);
	virtual void takeDamage(int dmg);
	virtual eEntityType getEntityType();
	virtual void setSpeed(float speed);
	virtual void synchronize();
	virtual void setMaxHealth(float health);

	//particle functions
	cEmitter* playerParticleEmitter;
	void setPlayerEmitterToActive();
	void emitParticles(float deltaTime, int shaderProgram);


	void updateEmitterPosition();
	void setProjectileInUse(sProjectile& projectile);
	void setProjectileRange();
	void resetProjectiles();
	void checkProjectileBounds();
	int getNextProjectileIndex();

	std::vector<sProjectile> projectilePool;
	std::vector<sProjectile> projectiles;
	std::vector<cGameObject*> projectilesToDraw;

private:
	void setPlayerForward();
};

#endif
