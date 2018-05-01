#include "cPlayer.h"
#include "cGameObject.h"
#include "cEmitter.h"
#include <iPhysicsFactory.h>
#include "cMathHelper.h"
#include "Utilities.h"
#include "RenderUtilities.h"

extern nPhysics::iPhysicsWorld* theWorld;
extern nPhysics::iPhysicsFactory* theFactory;

cPlayer::cPlayer() {
	this->thePlayerObject = new cGameObject();
	this->playerSpeed = 0.0f;
	this->rotationSpeed = 0.0f;
	this->playerForward = glm::vec3(1.0f, 0.0f, 0.0f);
	this->maxHealth = 100.0f;
	this->currentHealth = 100.0f;
	this->isInExplosionRadius = false;
	this->projectileRange = 12.f;
	this->SetBodyType(nPhysics::eCollisionBodyType::PLAYER_BODY);
	playerParticleEmitter = new cEmitter();
	//for projectiles
	glm::vec3 forwardVector;
	projectileIndex = -1;
}

void cPlayer::createProjectiles() {
	cGameObject* tempObject;

	for (int i = 0; i < 50; i++) {
		tempObject = new cGameObject();
		tempObject->meshName = "Sphere_xyz_n_uv.ply";
		tempObject->position = glm::vec3(100.f);
		tempObject->bIsWireFrame = true;
		tempObject->bIsSkyBoxObject = false;
		tempObject->diffuseColour = glm::vec4(0.f, 0.f, 0.f, 1.f);
		tempObject->scale = 0.25f;
		tempObject->radius = 0.125f;

		sProjectile tempProjectile;
		tempProjectile.damage = 10.f;
		tempProjectile.object = tempObject;
		tempProjectile.speed = this->playerSpeed + 2.f;
		tempProjectile.inUse = false;
		tempProjectile.projectileRange = projectileRange;
		tempProjectile.direction = playerForward;
		this->projectilePool.push_back(tempProjectile);
	}
}

void cPlayer::takeDamage(int dmg) {
	this->currentHealth -= dmg;
}

void cPlayer::setMaxHealth(float health) {
	this->maxHealth = health;
	this->currentHealth = maxHealth;
}

eEntityType cPlayer::getEntityType() {
	return eEntityType::PLAYER_ENTITY;
}

cPlayer::~cPlayer() {
	delete this->thePlayerObject;
}

void cPlayer::move(glm::vec3 directionToMove) {
	//this->thePlayerObject->theBody->ApplyImpulse(directionToMove * this->playerSpeed * (float)1.f / 60.f);
	this->thePlayerObject->position += directionToMove * this->playerSpeed * (float)1.f / 60.f;
	nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(this->thePlayerObject->theBody);
	tempBody->SetPosition(this->thePlayerObject->position);
}

void cPlayer::rotatePlayer(glm::vec3 directionToRotate) {
	//TODO::rotate the object directly
	this->thePlayerObject->orientation2.y += directionToRotate.y * this->rotationSpeed * (float)1.f / 60.f;
	nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(this->thePlayerObject->theBody);
	tempBody->SetRotation(this->thePlayerObject->orientation2);

	//get the new forward
	setPlayerForward();
}

int cPlayer::getNextProjectileIndex() {
	int index = -1;
	//check to see what projectile index will be

	for (int i = 0; i < this->projectilePool.size(); i++) {
		if (this->projectilePool[i].inUse == false){
			index = i;
			this->projectilePool[i].inUse = true;
			break;
		}
	}

	return index;
}

void cPlayer::setProjectileInUse(sProjectile& projectile) {
	for (int i = 0; i < this->projectilePool.size(); i++) {
		//if the memory addresses match set the projectile to be in use
		if (this->projectilePool[i].object == projectile.object){
			this->projectilePool[i].inUse = true;
		}
	}
}

void cPlayer::setPlayerForward() {
	//gets the players forward vector
	Matrix RotationMatrix = IDENTITY_MATRIX;
	RotateAboutY(&RotationMatrix, this->thePlayerObject->orientation2.y);
	glm::vec3 playerforwardVector = glm::vec3(1.0f, 0.0f, 0.0f);
	playerforwardVector = VectorMultiplyMatrices(&RotationMatrix, &playerforwardVector);
	this->playerForward = playerforwardVector;
}

void cPlayer::attack(glm::vec3 direction, float deltaTime) {
	//get the next available index of projectiles
	int index = getNextProjectileIndex();

	//add the projectile to the draw vector
	if (index > -1) {
		sProjectile tempProjectile = this->projectilePool[index];
		tempProjectile.direction = this->playerForward;
		//add the projectile to the active ones
		//create the rigid body and add it to the world
		nPhysics::iShape* shape = theFactory->CreateSphere(tempProjectile.object->scale / 2.f);
		nPhysics::sRigidBodyDesc desc;
		//set the position for the object and projectil
		desc.Position = this->thePlayerObject->position + this->playerForward;
		tempProjectile.object->position = this->thePlayerObject->position + this->playerForward;
		tempProjectile.projectileInitialPosition = tempProjectile.object->position;
		desc.Mass = 1.f;
		desc.Rotation = glm::vec3(0.0f);
		desc.Velocity = glm::vec3(0.0f);
		desc.noCollisionResponse = true;
		//give the projectile a rigid body
		tempProjectile.object->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::PLAYER_PROJECTILE_BODY);
		//add the projectile to vector
		this->projectiles.push_back(tempProjectile);
		//change the body
		nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(this->thePlayerObject->theBody);
		//set the gravity and factor
		//glm::vec3 tempVel;
		//tempBody->GetLinearVelocity(tempVel);
		tempBody->SetLinearVelocity(direction * tempProjectile.speed);
		tempBody->SetGravity(glm::vec3(0.f));
		//add the projectile rigid body to the world
		theWorld->AddCollisionBody(tempProjectile.object->theBody);

		//add the object to the objects to draw
		this->projectilesToDraw.push_back(tempProjectile.object);
	}
	else {
		this->resetProjectiles();
	}
}

void cPlayer::emitParticles(float deltaTime, int shaderProgram) {
	std::string name = "Sphere_xyz_n_uv.ply";
	this->playerParticleEmitter->position = this->thePlayerObject->position + -this->playerForward * 2.f;
}

void cPlayer::setPlayerEmitterToActive() {
	this->playerParticleEmitter->setEmitterToActive(true);
}

void cPlayer::updateEmitterPosition() {
	this->playerParticleEmitter->position = this->thePlayerObject->position + -playerForward;
}

void cPlayer::removeProjectile(cGameObject* theProjectile) {
	for (std::vector<cGameObject*>::iterator it = this->projectilesToDraw.begin(); it != this->projectilesToDraw.end(); it++) {
		//remove the projectile from the drawing vector
		if (*it == theProjectile) {
			it = this->projectilesToDraw.erase(it);
			break;
		}
	}

	//remove the projectile from the projetiles vector
	for (std::vector<sProjectile>::iterator it = this->projectiles.begin(); it != this->projectiles.end(); it++) {
		//remove the projectile from the drawing vector
		if ((*it).object == theProjectile) {
			nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>((*it).object->theBody);
			//remove the body from the world and delete it
			theWorld->RemoveRigidBody(tempBody);
			delete (*it).object->theBody;
			it = this->projectiles.erase(it);
			break;
		}
	}
}

void  cPlayer::setProjectileRange() {
	//set the projectile range
	for (int i = 0; i < this->projectilePool.size(); i++) {
		this->projectilePool[i].projectileRange = this->projectileRange;
	}
}

void cPlayer::resetProjectiles() {
	//reset the projectiles
	for (int i = 0; i < this->projectilePool.size(); i++) {
		this->projectilePool[i].inUse = false;
	}
}

void cPlayer::setSpeed(float speed) {
	this->playerSpeed = speed;
	this->rotationSpeed = speed / 2.f;
}

void cPlayer::synchronize() {
	//synchronize the mesh and the projectile meshes
	this->thePlayerObject->synchronize();
	for (int i = 0; i < this->projectilesToDraw.size(); i++) {
		this->projectilesToDraw[i]->synchronize();
	}
}

void cPlayer::applyContinuousDirectionalVelocityToProjectiles() {
	for (int i = 0; i < this->projectilesToDraw.size(); i++) {
		nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(this->projectilesToDraw[i]->theBody);
		sProjectile tempProjectile = this->projectiles[i];
		tempBody->SetLinearVelocity(tempProjectile.direction* tempProjectile.speed);
	}
}

void cPlayer::checkProjectileBounds() {
	std::vector<bool> toRemove;

	for (int i = 0; i < this->projectiles.size(); i++) {
		if (glm::distance(this->projectiles[i].projectileInitialPosition, this->projectiles[i].object->position) >= this->projectileRange) {
			toRemove.push_back(true);
		}
		else {
			toRemove.push_back(false);
		}
	}

	//remove the projectiles
	for (int i = 0; i < toRemove.size(); i++) {
		if (toRemove[i] == true) {
			this->removeProjectile(projectilesToDraw[i]);
		}
	}
}