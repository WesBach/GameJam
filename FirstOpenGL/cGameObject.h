#ifndef _cGameObject_HG_
#define _cGameObject_HG_
#include "AABB_Center_Radius.h"
#include "sMeshDrawInfo.h"
#include "sTextureBindBlendInfo.h"
#include "sMeshDrawInfo.h"
#include "iEntity.h"
#include <iCollisionBody.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "cPowerUp.h"
#include <string>
#include <vector>

class cAnimationManager;
class cPathingNode;
class iState;
class cEmitter;

enum eTypeOfObject
{	// Ok people, here's the deal:
	SPHERE,		// - it's a sphere
	PLANE,		// - it's a plane
	CAPSULE,    // - it's a capsule
	AAbb,
	BOX,
	CYLINDER,
	CONE,
	UNKNOWN = 99	// I don't know
};

enum eSpeed {
	Walk,
	Run,
	IdleSpeed
};

class cGameObject
{
public:
	cGameObject();		// constructor
	~cGameObject();		// destructor
	glm::vec3 position;
	glm::vec3 previousPosition;
	glm::vec3 orientation;
	glm::vec3 orientation2;
	float scale;
	AABB_Center_Radius* theBoundingBox;
	iState* theAIState;
	iEntity* theEntity;
	// **********************************
	// Add some physics things
	glm::vec3 vel;			// Velocity
	glm::vec3 accel;		// Acceleration
	bool bHasAABB;
	//glm::vec3 offset;
	nPhysics::iCollisionBody* theBody = NULL;
	void synchronize();

	cPathingNode* pathingNode;
	std::string friendlyName;
	// Refers to the enum above
	eTypeOfObject typeOfObject;		// (really an int)
	ePowerUpType typeOfPowerUp;
	//======================================
	//put all this in another class
	//======================================
	eSpeed theMovementSpeed = eSpeed::IdleSpeed;
	cAnimationManager* pAnimationManager;
	//=======================================
	float radius;
	float reflectRatio;
	float refractRatio;
	// **********************************
	bool bIsWireFrame;
	bool bIsSkyBoxObject;
	bool bIsReflectRefract;
	bool isNoiseGenerating;
	bool bIstoonShaded;
	glm::vec4 diffuseColour;
	glm::vec3 meshExtents;

	std::string meshName;		// mesh I'd like to draw
	std::vector<sTextureBindBlendInfo> vecMehs2DTextures;
	std::vector<sTextureBindBlendInfo> vecMeshCubeMaps;
	std::vector<std::string> vecMeshNames;
	sTextureBindBlendInfo* heightMap;
	sMeshDrawInfo drawInfo;
	cEmitter* theParticleEmitter;

	void updateRigidBody();
};

#endif
