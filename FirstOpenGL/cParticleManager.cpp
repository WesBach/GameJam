#include "cParticleManager.h"
#include "cGameObject.h"
#include "Utilities.h"
#include "RenderUtilities.h"
#include "cEnemy.h"
#include "cPlayer.h"
#include <GLFW\glfw3.h>

extern GLFWwindow* g_pGLFWWindow;

void cParticleManager::createEmitters(int numEmitters) {
	//clear the emitters
	this->mEnemyEmitters.clear();
	//create new ones
	for (int i = 0; i < numEmitters; i++) {
		this->mEnemyEmitters.push_back(new cEmitter());
	}

	this->mPlayerEmitter = new cEmitter();
}

void  cParticleManager::connectEmittersWithEntities(std::vector<cEnemy>& enemies) {
	//give them their emitter index
	//make sure there are enough emitters for the enemies
	if (this->mEnemyEmitters.size() >= enemies.size()) {
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i].particleManagerEmitterIndex = i;
		}
	}
}

void cParticleManager::addEmitter(cEmitter* emitter) {
	//make sure the emitter doesn't already exist in the vector
	bool isAlreadyAdded = false;
	for (int i = 0; i < this->mEnemyEmitters.size(); i++) {
		if (this->mEnemyEmitters[i] == emitter) {
			isAlreadyAdded = true;
			break;
		}
	}
	//add it if it isn't alreadt present
	if (isAlreadyAdded == false) {
		this->mEnemyEmitters.push_back(emitter);
	}
}

void cParticleManager::setEnemyEmitterToActive(int index) {
	if (index < this->mEnemyEmitters.size()) {
		cEmitter* emitter = this->mEnemyEmitters[index];
		//make sure the emitter isn't already in the vector
		bool emitterIsAlreadyActive = false;
		for (int i = 0; i < this->mActiveEmitters.size(); i++) {
			if (this->mActiveEmitters[i] == emitter) {
				emitterIsAlreadyActive = true;
				break;
			}
		}

		//if the emitter isn't in the vector add it
		//if (emitterIsAlreadyActive == false) {
		//	//find the emitter and add it to the active emitters vector(ensures no foreign emitters)
		//	for (int i = 0; i < this->mEnemyEmitters.size(); i++) {
		//		if (this->mEnemyEmitters[i] == emitter) {
		//			//initialize the emitter (50 particles, 25 created per step max)
		//			this->mEnemyEmitters[i]->init(50, 50,
		//				glm::vec3(0.1f, 0.1f, 0.1f),	// Min init vel
		//				glm::vec3(0.5f, 3.0f, 0.5f),	// max init vel
		//				5.0f, 10.0f,
		//				glm::vec3(0.0f, 0.0f, 0.0f),
		//				glm::vec3(1.0f, 1.0f, 1.0f),
		//				glm::vec3(0.0f, 0.0f, 0.0f),);
		//			//add the emitter to active
		//			this->mActiveEmitters.push_back(this->mEnemyEmitters[i]);
		//			break;
		//		}
		//	}
		//}
	}
}

void cParticleManager::setPlayerEmitterToActive() {
	bool emitterIsAlreadyActive = false;
	//check to see if the emitter is already active
	for (int i = 0; i < this->mActiveEmitters.size(); i++) {
		if (this->mActiveEmitters[i] == this->mPlayerEmitter) {
			emitterIsAlreadyActive = true;
			break;
		}
	}

	//if (emitterIsAlreadyActive == false) {
	//	//add the emitter to active
	//	this->mPlayerEmitter->init(50, 50,
	//		glm::vec3(0.1f, 0.1f, 0.1f),	// Min init vel
	//		glm::vec3(0.5f, 3.0f, 0.5f),	// max init vel
	//		5.0f, 10.0f,
	//		glm::vec3(0.0f, 0.0f, 0.0f),
	//		glm::vec3(1.0f, 1.0f, 1.0f),
	//		glm::vec3(0.0f, 0.0f, 0.0f));
	//	this->mActiveEmitters.push_back(this->mPlayerEmitter);
	//}
}

void cParticleManager::setPlayerEmitterToInactive() {
	//check to see if the emitter is active
	bool isInactive = true;
	for (int i = 0; i < this->mActiveEmitters.size(); i++) {
		if (this->mActiveEmitters[i] == this->mPlayerEmitter) {
			isInactive = false;
			break;
		}
	}

	//the emitter is active so remove it
	if (isInactive == false) {
		//remove the emitter
		for (std::vector<cEmitter*>::iterator it = this->mActiveEmitters.begin(); it != this->mActiveEmitters.end(); it++) {
			if (*it == this->mPlayerEmitter) {
				it = this->mActiveEmitters.erase(it);
				break;
			}
		}
	}
}

std::vector<cEmitter*>& cParticleManager::getActiveEmitters() {
	return this->mActiveEmitters;
}

void cParticleManager::deactivateEmitter(int index) {
	//remove the emitter from the active vector
	if (index < this->mEnemyEmitters.size())
	{
		cEmitter* theEmitter = this->mEnemyEmitters[index];

		for (std::vector<cEmitter*>::iterator it = this->mActiveEmitters.begin(); it != this->mActiveEmitters.end(); it++) {
			//make sure it's the right one
			if ((*it) == theEmitter) {
				it = this->mActiveEmitters.erase(it);
				break;
			}
		}
	}
}

void cParticleManager::clearActiveEmitter() {
	//clear the active emitter vector
	this->mActiveEmitters.clear();
}

void cParticleManager::clearAllEmitters() {
	//clear all emitters
	this->mActiveEmitters.clear();
	this->mEnemyEmitters.clear();
}

void cParticleManager::updateLivingParticles(float deltaTime) {
	//update the particles
	for (int i = 0; i < this->mActiveEmitters.size(); i++) {
		this->mActiveEmitters[i]->Update(deltaTime, glm::vec3(0.f));
	}
}

void cParticleManager::drawActiveParticles(int shaderId) {
	//get the active particles for each active emitter and draw that amount of particles to the screen
	cGameObject* tempObject = new cGameObject();
	tempObject = new cGameObject();
	tempObject->meshName = "Sphere_xyz_n_uv.ply";
	tempObject->bIsWireFrame = false;
	tempObject->bIsSkyBoxObject = false;
	//make slightly transparent
	tempObject->diffuseColour = glm::vec4(1.f, 1.f, 1.f, 0.5f);
	tempObject->scale = 0.25f;

	//TODO:: may have to change this
	//might kill performance
	for (int i = 0; i < this->mActiveEmitters.size(); i++) {
		//get the active particles
		std::vector<cParticle*> activeParticles;
		int numActiveParticles = this->mActiveEmitters[i]->getLivingParticles(activeParticles);
		std::vector<cGameObject*> theParticlesToDraw;

		//draw each particle individually
		for (int particleIndex = 0; particleIndex < numActiveParticles; particleIndex++) {
			tempObject->position = activeParticles[particleIndex]->pos;
			float textureBlend = getRandInRange(0.0f, 1.0f);
			float texBlend2 = 1.0f - textureBlend;
			//set the textures with random blend values
			tempObject->vecMehs2DTextures.push_back(sTextureBindBlendInfo("smoke.bmp", textureBlend));
			tempObject->vecMehs2DTextures.push_back(sTextureBindBlendInfo("smoke2.bmp", texBlend2));
			DrawObject(tempObject, shaderId, 0.f, true, numActiveParticles);
			//clear the textures
			tempObject->vecMehs2DTextures.clear();
		}

		//render the objects
		renderScene(theParticlesToDraw, g_pGLFWWindow, 0, 0.f);
	}

	//clean up the object
	delete tempObject;
}

cParticleManager::cParticleManager() {
	this->mPlayerEmitter = new cEmitter();
}

cParticleManager::~cParticleManager() {
	delete this->mPlayerEmitter;
}

void cParticleManager::updateEmitterPositions(std::vector<cEnemy>& enemies, cPlayer* thePlayer) {
	//update the emitter position for the player
	this->mPlayerEmitter->position = thePlayer->thePlayerObject->position;
	//update the emitter position for the enemies
	if (enemies.size() <= this->mEnemyEmitters.size()) {
		for (int i = 0; i < enemies.size(); i++) {
			this->mEnemyEmitters[enemies[i].particleManagerEmitterIndex]->position = enemies[i].theEnemyObject->position;
		}
	}
}