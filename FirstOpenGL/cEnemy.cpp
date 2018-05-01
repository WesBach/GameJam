#include "cEnemy.h"
#include "cGameObject.h"
#include <iPhysicsFactory.h>

void cEnemy::attack(glm::vec3 direction, float deltaTime) {
	if (this->attackType == eAttackType::PROJECTILE){
		//add the time to counter
		this->timeElapsedBetweenProjectiles += deltaTime;
		//get the projectile
		sProjectile projectile = this->createProjectileBasedOnEnemyStats();

		//check projectile state
		for (int i = 0; i < this->projectilePool.size(); i++) { //go through the pool
			//try to find the projectile in the projectiles vector
			bool foundCurrentProjectile = false;

			for (int index = 0; index < this->projectiles.size(); index++) {
				if (this->projectilePool[i].object == this->projectiles[index].object) {
					foundCurrentProjectile = true;
				}
			}
			//if we didn't find the projectile set in use to false
			if (foundCurrentProjectile == false) {
				this->projectilePool[i].inUse = false;
			}
		}

		if (projectile.object != NULL) {
			if (this->timeElapsedBetweenProjectiles >= 0.5f) {
				//set the direction for the projectile
				projectile.direction = direction;
				projectile.object->position = this->theEnemyObject->position + projectile.direction;
				projectile.damage = 10.f;
				projectile.object->diffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

				//push back the projectile
				this->projectilesToDraw.push_back(projectile.object);
				this->projectiles.push_back(projectile);
				//reset the time
				this->timeElapsedBetweenProjectiles = 0.0f;
			}
		}
	}
	else if (this->attackType == eAttackType::EXPLOSION) {
		//probably just a sphere getting bigger where the enemy was
		this->timeElapsedBetweenProjectiles += deltaTime;
		//get the projectile
		sProjectile projectile = this->createProjectileBasedOnEnemyStats();
		//default check
		if (projectile.object != NULL) {
			//make sure the object only explodes once
			if (isExploding == false) {
				//set the position for the explosion
				projectile.object->position = this->theEnemyObject->position;
				projectile.object->diffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 0.5f);
				//make sure the explosion doesn't move
				projectile.object->scale = 3.f;
				projectile.speed = 0.0f;
				projectile.direction = glm::vec3(0.f);
				projectile.object->bIsWireFrame = false;
				//set the damage for the explosion
				projectile.damage = 40.f;
				//push back the projectile
				this->explosion.push_back(projectile.object);
				this->projectiles.push_back(projectile);
				this->isExploding = true;
			}
			else {
				if (this->explosion.size() != 0) {
					if (this->explosion[0]->scale < this->explosionSize) {
						//increase the scale of the explosion and dont move it
						this->explosion[0]->scale += deltaTime;
					}
				}
			}
		}
	}
}

cEnemy::cEnemy() {
	this->direction = glm::vec3(0.f);
	this->projectileRange = 10.f;
	this->timeElapsedBetweenProjectiles = 0.5f;
	this->health = 100;
	this->maxHealth = 100;
	this->explosionSize = 10.f;
	this->isExploding = false;
	this->particleManagerEmitterIndex = -1;
	this->SetBodyType(nPhysics::eCollisionBodyType::ENEMY_BODY);

	for (int i = 0; i < 10; i++) {
		cGameObject* projectileObject = new cGameObject();
		projectileObject->meshName = "Sphere_xyz_n_uv.ply";
		projectileObject->position = glm::vec3(0.f);
		projectileObject->bIsWireFrame = true;
		projectileObject->bIsSkyBoxObject = false;
		projectileObject->diffuseColour = glm::vec4(1.f);
		projectileObject->scale = 0.25f;
		projectileObject->radius = 0.125f;

		sProjectile pro;
		pro.object = projectileObject;
		pro.speed = 12.f;
		pro.inUse = false;
		this->projectilePool.push_back(pro);
	}
}

cEnemy::~cEnemy() {

}

void cEnemy::takeDamage(int dmg) {
	this->health -= dmg;
}

void cEnemy::move(glm::vec3 directionToMove) {
	this->theEnemyObject->position += directionToMove * this->speed * (float)1.f / 60.f;
	nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(this->theEnemyObject->theBody);
	tempBody->SetPosition(this->theEnemyObject->position);
}

void cEnemy::setMaxHealth(float health) {
	this->maxHealth = health;
	this->health = maxHealth;
}

void cEnemy::removeProjectile(cGameObject* theProjectile) {
	for (std::vector<cGameObject*>::iterator it = this->projectilesToDraw.begin(); it != this->projectilesToDraw.end(); it++) {
		//remove the projectile from the drawing vector
		if (*it == theProjectile) {
			it = this->projectilesToDraw.erase(it);
			break;
		}
	}

	for (std::vector<sProjectile>::iterator it = this->projectiles.begin(); it != this->projectiles.end(); it++) {
		if ((*it).object == theProjectile) {
			it = this->projectiles.erase(it);
			break;
		}
	}

	for (int i = 0; i < this->projectilePool.size(); i++)
	{
		if (this->projectilePool[i].object == theProjectile)
		{
			this->projectilePool[i].inUse = false;
		}
	}
}

void cEnemy::removeExplosion(cGameObject* theExplosion) {
	for (std::vector<cGameObject*>::iterator it = this->explosion.begin(); it != this->explosion.end(); it++) {
		//remove the projectile from the drawing vector
		if (*it == theExplosion) {
			it = this->explosion.erase(it);
			break;
		}
	}

	for (std::vector<sProjectile>::iterator it = this->projectiles.begin(); it != this->projectiles.end(); it++) {
		if ((*it).object == theExplosion) {
			it = this->projectiles.erase(it);
			break;
		}
	}

	for (int i = 0; i < this->projectilePool.size(); i++)
	{
		if (this->projectilePool[i].object == theExplosion)
		{
			this->projectilePool[i].inUse = false;
		}
	}
}

int cEnemy::getNextProjectileIndex() {
	int index = -1;

	for (int i = 0; i < this->projectilePool.size(); i++) {
		if (this->projectilePool[i].inUse == false) {
			index = i;
			this->projectilePool[i].inUse = true;
			break;
		}
	}

	return index;
}

sProjectile cEnemy::createProjectileBasedOnEnemyStats() {
	sProjectile tempObject;
	int index = this->getNextProjectileIndex();

	//get the projectile
	if (index >= 0)
	{
		tempObject = this->projectilePool[index];
	}
	else {
		return sProjectile();
	}

	return tempObject;
}

eEntityType cEnemy::getEntityType() {
	return eEntityType::ENEMY_ENTITY;
}

void cEnemy::setSpeed(float speed) {
	this->speed = speed;
}

void cEnemy::synchronize() {
	for (int i = 0; i < this->projectilesToDraw.size(); i++) {
		this->projectilesToDraw[i]->synchronize();
	}
}

void cEnemy::checkProjectileBounds() {
}