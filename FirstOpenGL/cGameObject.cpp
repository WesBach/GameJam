#include "cGameObject.h"
#include "cPathingNode.h"
#include "cEmitter.h"
#include <iRigidBody.h>

cGameObject::cGameObject()
{
	this->scale = 1.0f;	// (not zero)
	this->position = glm::vec3(0.0f);
	this->previousPosition = glm::vec3(0.0f);
	this->orientation = glm::vec3(0.0f);
	this->orientation2 = glm::vec3(0.0f);
	this->vel = glm::vec3(0.0f);
	this->accel = glm::vec3(0.0f);	
	this->pathingNode = new cPathingNode();
	this->friendlyName = "";
	// If you aren't sure what the 4th value should be, 
	this->diffuseColour = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	this->reflectRatio = 0.0f;
	this->refractRatio = 0.0f;
	//Assume everything is simulated 
	this->theBoundingBox = new AABB_Center_Radius();
	this->bHasAABB = false;
	this->radius = 0.0f;	// Is this the best value??? Who knows?
	this->bIsReflectRefract = false;
	this->typeOfObject = eTypeOfObject::UNKNOWN;	// Technically non-standard
	this->pAnimationManager = NULL;
	this->bIsWireFrame = false;
	this->bIsSkyBoxObject = false;
	this->isNoiseGenerating = false;
	this->heightMap = NULL;
	this->bIstoonShaded = true;
	this->theParticleEmitter = new cEmitter();
	return;
}

cGameObject::~cGameObject()
{
	delete this->theBoundingBox;
	delete this->pathingNode;
	delete this->pAnimationManager;
	return;
}

void cGameObject::synchronize() {
	nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(this->theBody);
	tempBody->GetPosition(this->position);
	tempBody->GetVelocity(this->vel);
	//tempBody->GetRotation(this->orientation2);
}

void cGameObject::updateRigidBody() {
	nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(this->theBody);
	tempBody->SetPosition(this->position);
}