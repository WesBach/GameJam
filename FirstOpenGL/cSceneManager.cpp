#include "cSceneManager.h"
#include "cVAOMeshManager.h"
#include "cGameObject.h"
#include "cMesh.h"

#include "cAngryState.h"
#include "cCuriousState.h"
#include "cFollowState.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cPowerUp.h"
#include "cEmitter.h"
#include "Utilities.h"
#include "cShieldDescription.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iPhysicsFactory.h>

extern nPhysics::iPhysicsWorld* theWorld;
extern nPhysics::iPhysicsFactory* theFactory;
extern sScene* g_pCurrentScene;
extern glm::vec3 g_cameraXYZ;

const glm::vec4 brown(0.611f, 0.470f, 0.149f, 1.0f);
const glm::vec4 green(0.133f, 0.505f, 0.152f, 1.0f);
const glm::vec4 purple(0.933f, 0.090f, 0.901f, 1.0f);
const glm::vec4 orange(0.933f, 0.415f, 0.090f, 1.0f);
const glm::vec4 red(0.933f, 0.192f, 0.090f, 1.0f);

sScene::sScene() {
	this->currentPlayer = new cPlayer();
	//this->playerShield = new cShieldDescription(3);
}

sScene::~sScene() {
	//delete this->playerShield;
	//delete this->currentPlayer;
}

void sScene::setPlayerPointer() {
	for (int i = 0; i < this->entities.size(); i++) {
		if (this->entities[i]->theEntity->getEntityType() == eEntityType::PLAYER_ENTITY) {
			this->currentPlayer->thePlayerObject = this->entities[i];
		}
	}
}

bool  cSceneManager::loadSceneFromFileIntoSceneMap(std::string& fileName, int mapIndex, cVAOMeshManager* theMeshManager) {
	sScene* tempScene = new sScene();
	tempScene->currentPlayer = new cPlayer();

	std::ifstream sceneInfo(fileName.c_str());
	int numPlayers = 0;
	int numEnemies = 0;
	int numTerrain = 0;
	if (!sceneInfo.is_open())
	{	// Didn't open file, so return
		return false;
	}

	ReadFileToToken(sceneInfo, "CAMERA_POSITION");
	sceneInfo >> g_cameraXYZ.x;
	sceneInfo >> g_cameraXYZ.y;
	sceneInfo >> g_cameraXYZ.z;

	//popualate the players
	ReadFileToToken(sceneInfo, "NUMBER_OF_ENTITIES");
	sceneInfo >> numPlayers;
	ReadFileToToken(sceneInfo, "ENTITIES");
	for (int i = 0; i < numPlayers; i++)
	{
		loadObjectData(sceneInfo, tempScene->entities, theMeshManager);
	}

	ReadFileToToken(sceneInfo, "PLAYER_SPEED");
	float playerSpeed;
	sceneInfo >> playerSpeed;

	//populate the terrain
	ReadFileToToken(sceneInfo, "NUMBER_OF_TERRAIN");
	sceneInfo >> numTerrain;
	ReadFileToToken(sceneInfo, "TERRAIN");
	for (int i = 0; i < numTerrain; i++)
	{
		loadObjectData(sceneInfo, tempScene->terrain, theMeshManager);
	}

	//populate the enemies
	ReadFileToToken(sceneInfo, "ENEMY_SPEED");
	float enemySpeed;
	sceneInfo >> enemySpeed;

	for (int i = 0; i < tempScene->entities.size(); i++) {
		if (tempScene->entities[i]->theAIState != NULL) {
			eStateType type = tempScene->entities[i]->theAIState->getStateType();
			if (type == eStateType::ANGRY) {
				cAngryState* theState = dynamic_cast<cAngryState*>(tempScene->entities[i]->theAIState);
				theState->speed = enemySpeed;
			}
			else if (type == eStateType::FOLLOWER) {
				cFollowState* theState = dynamic_cast<cFollowState*>(tempScene->entities[i]->theAIState);
				theState->speed = enemySpeed;
			}
			else if (type == eStateType::CURIOUS) {
				cCuriousState* theState = dynamic_cast<cCuriousState*>(tempScene->entities[i]->theAIState);
				theState->speed = enemySpeed;
			}
		}
	}

	//populate the enemies
	int numPowerups;
	ReadFileToToken(sceneInfo, "NUMBER_OF_POWERUPS");
	sceneInfo >> numPowerups;
	ReadFileToToken(sceneInfo, "POWERUPS");

	for (int i = 0; i < numPowerups; i++)
	{
		loadObjectData(sceneInfo, tempScene->powerUps, theMeshManager);
	}

	//get the player speed
	for (int i = 0; i < tempScene->entities.size(); i++) {
		if (tempScene->entities[i]->theEntity->getEntityType() == eEntityType::PLAYER_ENTITY) {
			tempScene->currentPlayer->setSpeed(playerSpeed);
			tempScene->currentPlayer->thePlayerObject = tempScene->entities[i];
			tempScene->currentPlayer->setPlayerEmitterToActive();
			//set player projectiles
			tempScene->currentPlayer->createProjectiles();
			tempScene->currentPlayer->resetProjectiles();
		}
		else if (tempScene->entities[i]->theEntity->getEntityType() == eEntityType::ENEMY_ENTITY) {
			tempScene->entities[i]->theEntity->setSpeed(enemySpeed);
		}
	}

	//close the file stream
	sceneInfo.close();
	//add the scene to the map
	this->mSceneMap[mapIndex] = tempScene;

	//TODO:: when adding more levels do this per level
	//TODO:: will have to remove the old rigid bodies as well
	loadAssimpLoadedModelsIntoScene(tempScene);
	createSceneRigidBodies(tempScene, theMeshManager);
	loadConstraintsForScene(tempScene);
	this->numLevels = this->mSceneMap.size() - 1;
	return true;
}

bool cSceneManager::loadModelsFromModelInfoFile(std::string& filename, cVAOMeshManager* theMeshManager, int shaderId) {
	std::ifstream modelNameFile(filename.c_str());
	std::string tempString;
	int numModels = 0;
	int numModelsUV;
	bool succeeded = true;

	if (!modelNameFile.is_open())
	{	// Didn't open file, so return
		return false;
	}

	ReadFileToToken(modelNameFile, "NUM_MODELS_UV");
	modelNameFile >> numModelsUV;
	ReadFileToToken(modelNameFile, "MODEL_NAME_UV_START");

	for (int i = 0; i < numModelsUV; i++)
	{
		modelNameFile >> tempString;
		cMesh* testMesh;
		testMesh = new cMesh();
		testMesh->name = tempString;

		if (testMesh->name == "SmoothSphere_Inverted_Normals_xyz_n.ply")
		{
			if (!loadPlyFileIntoMeshWithNormals(tempString, *testMesh))
			{
				//std::cout << "Didn't load model" << std::endl;
				std::cout << "Didn't load model >" << testMesh->name << "<" << std::endl;
				succeeded = false;
			}
		}
		else {
			if (!loadPlyFileIntoMeshWith_Normals_and_UV(tempString, *testMesh))
			{
				std::cout << "Didn't load model >" << testMesh->name << "<" << std::endl;
				succeeded = false;
			}
		}

		if (!theMeshManager->loadMeshIntoVAO(*testMesh, shaderId, true))
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			std::cout << "Could not load mesh >" << testMesh->name << "< into VAO" << std::endl;
			succeeded = false;
		}

		tempString = "";
	}

	return succeeded;
}

sScene cSceneManager::getSceneById(int id) {
	std::vector<cGameObject> objects;

	//make sure the map has a size greater 0
	if (this->mSceneMap.size() > 0)
	{
		return *this->mSceneMap[id];
	}
}

void cSceneManager::copySceneFromCopyToPointer(const sScene& copyFrom, sScene* copyTo) {
	sScene temp = copyFrom;
	//clear the previous scene
	copyTo->entities.clear();
	copyTo->terrain.clear();
	//make new objects with the game object copies
	for (int i = 0; i < temp.entities.size(); i++)
	{
		copyTo->entities.push_back(new cGameObject(*temp.entities[i]));
	}

	for (int i = 0; i < temp.terrain.size(); i++)
	{
		copyTo->terrain.push_back(new cGameObject(*temp.terrain[i]));
	}

	for (int i = 0; i < temp.powerUps.size(); i++)
	{
		copyTo->powerUps.push_back(new cGameObject(*temp.powerUps[i]));
	}

	copyTo->currentPlayer = new cPlayer(*copyFrom.currentPlayer);
}

void cSceneManager::loadObjectData(std::ifstream& theFile, std::vector<cGameObject*>& theVector, cVAOMeshManager* theMeshManager) {
	std::string physicsObjectType = "";
	std::string stateType = "";
	cGameObject* pTempGO = new cGameObject();
	//cPathingNode* pTempNode = new cPathingNode();
	float tempFloat = 0.0f;
	float mass = 0.0f;
	int entityType;
	int powerUpType;
	theFile >> pTempGO->meshName;
	theFile >> pTempGO->position.x;
	theFile >> pTempGO->position.y;
	theFile >> pTempGO->position.z;
	theFile >> pTempGO->scale;
	theFile >> pTempGO->diffuseColour.x;
	theFile >> pTempGO->diffuseColour.y;
	theFile >> pTempGO->diffuseColour.z;
	theFile >> pTempGO->diffuseColour.a;
	theFile >> pTempGO->bIsWireFrame;
	theFile >> tempFloat;
	pTempGO->orientation2.x = glm::radians(tempFloat);
	theFile >> tempFloat;
	pTempGO->orientation2.y = glm::radians(tempFloat);
	theFile >> tempFloat;
	pTempGO->orientation2.z = glm::radians(tempFloat);
	theFile >> physicsObjectType;
	theFile >> pTempGO->bHasAABB;
	theFile >> entityType;
	theFile >> stateType;
	theFile >> powerUpType;

	//get the physics object shape
	if (physicsObjectType == "UNKNOWN")
	{
		pTempGO->typeOfObject = eTypeOfObject::UNKNOWN;
	}
	else if (physicsObjectType == "SPHERE")
	{
		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
	}
	else if (physicsObjectType == "BOX") {
		pTempGO->typeOfObject = eTypeOfObject::BOX;
	}
	else if (physicsObjectType == "CYLINDER") {
		pTempGO->typeOfObject = eTypeOfObject::CYLINDER;
	}
	else if (physicsObjectType == "CONE") {
		pTempGO->typeOfObject = eTypeOfObject::CONE;
	}
	else
	{
		pTempGO->typeOfObject = eTypeOfObject::PLANE;
	}

	//set the entity type
	switch (entityType) {
	case eEntityType::PLAYER_ENTITY:
		pTempGO->theEntity = new cPlayer();
		break;
	case eEntityType::ENEMY_ENTITY:
		pTempGO->theEntity = new cEnemy();
		break;
	case eEntityType::BOSS_ENTITY:
		//TODO:: add the boss entity class
		break;
	}

	switch (powerUpType) {
	case ePowerUpType::POWERUP_HEALTH:
		pTempGO->typeOfPowerUp = ePowerUpType::POWERUP_HEALTH;
		break;
	case ePowerUpType::POWERUP_RANGE_INCREASE:
		pTempGO->typeOfPowerUp = ePowerUpType::POWERUP_RANGE_INCREASE;
		break;
	}

	//set the ai state
	if (stateType == "ANGRY") {
		pTempGO->theAIState = new cAngryState();
	}
	else if (stateType == "FOLLOWER") {
		pTempGO->theAIState = new cFollowState();
	}
	else if (stateType == "CURIOUS") {
		pTempGO->theAIState = new cCuriousState();
	}
	else {
		pTempGO->theAIState = NULL;
	}

	theVector.push_back(pTempGO);
}

bool cSceneManager::loadPlyFileIntoMeshWith_Normals_and_UV(std::string filename, cMesh &theMesh) {
	// Load the vertices
	std::string file = "assets\\models\\" + filename;
	// c_str() changes a string to a "c style char* string"
	std::ifstream plyFile(file.c_str());

	if (!plyFile.is_open())
	{	// Didn't open file, so return
		return false;
	}
	// File is open, let's read it

	ReadFileToToken(plyFile, "vertex");
	//	int numVertices = 0;
	plyFile >> theMesh.numberOfVertices;

	ReadFileToToken(plyFile, "face");
	//	int numTriangles = 0;
	plyFile >> theMesh.numberOfTriangles;

	ReadFileToToken(plyFile, "end_header");

	// Allocate the appropriate sized array (+a little bit)
	//theMesh.pVertices = new cVertex_xyz_rgb_n[theMesh.numberOfVertices];
	theMesh.pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[theMesh.numberOfVertices];
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for (int index = 0; index < theMesh.numberOfVertices; index++)
	{
		//end_header
		//-0.0312216 0.126304 0.00514924 0.850855 0.5
		float x, y, z, nx, ny, nz;
		// Added
		float u, v;		// Model now has texture coordinate

						// Typical vertex is now...
						// 29.3068 -5e-006 24.3079 -0.949597 0.1875 -0.251216 0.684492 0.5

		plyFile >> x >> y >> z;
		plyFile >> nx >> ny >> nz;
		//
		plyFile >> u >> v;			// ADDED

		theMesh.pVertices[index].x = x;	// vertices[index].x = x;
		theMesh.pVertices[index].y = y;	// vertices[index].y = y;
		theMesh.pVertices[index].z = z;
		theMesh.pVertices[index].r = 1.0f;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].g = 1.0f;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].b = 1.0f;	// vertices[index].r = 1.0f;
		theMesh.pVertices[index].nx = nx;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].ny = ny;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].nz = nz;	// vertices[index].r = 1.0f;

											// Only store the 1st UV.
		theMesh.pVertices[index].u1 = u;
		theMesh.pVertices[index].v1 = v;
	}

	// Load the triangle (or face) information, too
	for (int count = 0; count < theMesh.numberOfTriangles; count++)
	{
		// 3 164 94 98
		int discard = 0;
		plyFile >> discard;									// 3
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
	}

	//	theMesh.CalculateNormals();

	return true;
}

bool cSceneManager::loadPlyFileIntoMeshWithNormals(std::string filename, cMesh &theMesh) {
	// Load the vertices
	// c_str() changes a string to a "c style char* string"
	std::string file = "assets\\models\\" + filename;
	std::ifstream plyFile(file.c_str());

	if (!plyFile.is_open())
	{	// Didn't open file, so return
		return false;
	}
	// File is open, let's read it

	ReadFileToToken(plyFile, "vertex");
	//	int numVertices = 0;
	plyFile >> theMesh.numberOfVertices;

	ReadFileToToken(plyFile, "face");
	//	int numTriangles = 0;
	plyFile >> theMesh.numberOfTriangles;

	ReadFileToToken(plyFile, "end_header");

	// Allocate the appropriate sized array (+a little bit)
	//theMesh.pVertices = new cVertex_xyz_rgb_n[theMesh.numberOfVertices];
	theMesh.pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[theMesh.numberOfVertices];
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for (int index = 0; index < theMesh.numberOfVertices; index++)
	{
		//end_header
		//-0.0312216 0.126304 0.00514924 0.850855 0.5
		float x, y, z, nx, ny, nz;//, confidence, intensity;

		plyFile >> x;
		plyFile >> y;
		plyFile >> z;
		plyFile >> nx >> ny >> nz;
		//		plyFile >> confidence;
		//		plyFile >> intensity;

		theMesh.pVertices[index].x = x;	// vertices[index].x = x;
		theMesh.pVertices[index].y = y;	// vertices[index].y = y;
		theMesh.pVertices[index].z = z;
		theMesh.pVertices[index].r = 1.0f;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].g = 1.0f;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].b = 1.0f;	// vertices[index].r = 1.0f;
		theMesh.pVertices[index].nx = nx;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].ny = ny;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].nz = nz;	// vertices[index].r = 1.0f;
	}

	// Load the triangle (or face) information, too
	for (int count = 0; count < theMesh.numberOfTriangles; count++)
	{
		// 3 164 94 98
		int discard = 0;
		plyFile >> discard;									// 3
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
	}

	//	theMesh.CalculateNormals();

	return true;
}

void cSceneManager::populateEnemies(std::vector<cEnemy>& enemies, sScene* theScene) {
	//clear the vector
	enemies.clear();

	//populate the enemies
	for (int i = 0; i < theScene->entities.size(); i++) {
		eStateType stateType = theScene->entities[i]->theAIState->getStateType();
		eAttackType attackType;
		cEnemy enemy;
		//set the emitter position
		//enemy.theParticleEmitter->position = theScene->enemies[i]->position;

		//check the state of the object
		if (stateType == eStateType::FOLLOWER) {
			attackType = eAttackType::EXPLOSION;
			enemy.enemyType = eEnemyType::SUICIDE;
			enemy.health = 100.f;
			enemy.maxHealth = 100.f;
		}
		else {
			enemy.enemyType = eEnemyType::GUNNER;
			attackType = eAttackType::PROJECTILE;
			enemy.health = 150.f;
			enemy.maxHealth = 150.f;
		}

		//set the attack type
		enemy.attackType = attackType;
		enemy.theEnemyObject = theScene->entities[i];
		//add the enemy
		enemies.push_back(enemy);
	}
}

cSceneManager::cSceneManager() {
	this->currentLevel = 0;
	this->numLevels = 0;
}

void cSceneManager::loadLevelTextures(sScene* theScene) {
	//based on current level load textures
	switch (this->currentLevel) {
		//level 1
	case 0:
		for (int i = 0; i < theScene->entities.size(); i++)
		{
			if (theScene->entities[i]->meshName == "mig29_xyz.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("leaves.bmp", 1.0f));
			}
			else if (theScene->entities[i]->meshName == "Raider_ASCII_UVtex.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("lava.bmp", 1.0f));
			}
			else if (theScene->entities[i]->meshName == "Viper_MkVII_ASCII_UVTex.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("rust.bmp", 0.5f));
			}
			else if (theScene->entities[i]->meshName == "Sample_Ship.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("sh3.bmp", 1.0f));
			}
		}

		for (int i = 0; i < theScene->terrain.size(); i++)
		{
			if (theScene->terrain[i]->meshName == "MeshLabTerrain_FLAT_xyz_n_uv.ply")
			{
				theScene->terrain[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("realistic-wet-grassl.bmp", 1.f));
				//heightMap
				theScene->terrain[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("heightmap.bmp", 1.f));
				theScene->terrain[i]->heightMap = new  sTextureBindBlendInfo("heightmap.bmp", 1.f);
				theScene->terrain[i]->bIstoonShaded = false;
			}
			else if (theScene->terrain[i]->meshName == "MeshLabTerrain_FLAT_xyz_n_uv1.ply") {
				theScene->terrain[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("TropicalSunnyDayUp2048.bmp", 0.5f));
				theScene->terrain[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("heightmap.bmp", 1.f));
				theScene->terrain[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("water-texture.bmp", 0.5f));
				theScene->terrain[i]->bIstoonShaded = false;
				theScene->terrain[i]->isNoiseGenerating = true;
			}
			if (theScene->terrain[i]->meshName == "high_poly_sphere.ply")
			{
				theScene->terrain[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("blue.bmp", 1.f));
			}
			else if (theScene->terrain[i]->meshName == "SmoothSphere_Inverted_Normals_xyz_n.ply")
			{
				theScene->terrain[i]->vecMeshCubeMaps.push_back(sTextureBindBlendInfo("sunny", 1.0f));
				theScene->terrain[i]->bIsSkyBoxObject = true;
			}
		}

		for (int i = 0; i < theScene->powerUps.size(); i++)
		{
			if (theScene->powerUps[i]->meshName == "health_pack.ply")
			{
				theScene->powerUps[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("health.bmp", 1.0f));
			}
			if (theScene->powerUps[i]->meshName == "bullet.ply")
			{
				theScene->powerUps[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("gold.bmp", 1.0f));
			}
		}
		break;

		//level 2
	case 1:
		for (int i = 0; i < theScene->entities.size(); i++)
		{
			if (theScene->entities[i]->meshName == "mig29_xyz.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("leaves.bmp", 1.0f));
			}
			else if (theScene->entities[i]->meshName == "Raider_ASCII_UVtex.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("lava.bmp", 1.0f));
			}
			else if (theScene->entities[i]->meshName == "Viper_MkVII_ASCII_UVTex.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("rust.bmp", 0.5f));
			}
			else if (theScene->entities[i]->meshName == "Sample_Ship.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("sh3.bmp", 1.0f));
			}
		}

		for (int i = 0; i < theScene->terrain.size(); i++)
		{
			if (theScene->terrain[i]->meshName == "MeshLabTerrain_FLAT_xyz_n_uv.ply")
			{
				theScene->terrain[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("ground_grass.bmp", 0.5f));
				//heightMap
				theScene->terrain[i]->heightMap = new sTextureBindBlendInfo("heightmap.bmp", 1.f);
			}
			else if (theScene->terrain[i]->meshName == "SmoothSphere_Inverted_Normals_xyz_n.ply")
			{
				theScene->terrain[i]->vecMeshCubeMaps.push_back(sTextureBindBlendInfo("sunny", 1.0f));
				theScene->terrain[i]->bIsSkyBoxObject = true;
			}
		}

		for (int i = 0; i < theScene->powerUps.size(); i++)
		{
			if (theScene->powerUps[i]->meshName == "health_pack.ply")
			{
				theScene->powerUps[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("health.bmp", 1.0f));
			}
			if (theScene->powerUps[i]->meshName == "bullet.ply")
			{
				theScene->powerUps[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("gold.bmp", 1.0f));
			}
		}
		break;

		//level 3
	case 2:
		for (int i = 0; i < theScene->entities.size(); i++)
		{
			if (theScene->entities[i]->meshName == "mig29_xyz.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("leaves.bmp", 1.0f));
			}
			else if (theScene->entities[i]->meshName == "Raider_ASCII_UVtex.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("lava.bmp", 1.0f));
			}
			else if (theScene->entities[i]->meshName == "Viper_MkVII_ASCII_UVTex.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("rust.bmp", 0.5f));
			}
			else if (theScene->entities[i]->meshName == "Sample_Ship.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("sh3.bmp", 1.0f));
			}
		}

		for (int i = 0; i < theScene->terrain.size(); i++)
		{
			if (theScene->terrain[i]->meshName == "MeshLabTerrain_FLAT_xyz_n_uv.ply")
			{
				theScene->terrain[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("lava.bmp", 0.5f));
				//heightMap
				theScene->terrain[i]->heightMap = new sTextureBindBlendInfo("heightmap.bmp", 1.f);
			}
			else if (theScene->terrain[i]->meshName == "SmoothSphere_Inverted_Normals_xyz_n.ply")
			{
				theScene->terrain[i]->vecMeshCubeMaps.push_back(sTextureBindBlendInfo("space", 1.0f));
				theScene->terrain[i]->bIsSkyBoxObject = true;
			}
		}

		for (int i = 0; i < theScene->powerUps.size(); i++)
		{
			if (theScene->powerUps[i]->meshName == "health_pack.ply")
			{
				theScene->powerUps[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("health.bmp", 1.0f));
			}
			if (theScene->powerUps[i]->meshName == "bullet.ply")
			{
				theScene->powerUps[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("gold.bmp", 1.0f));
			}
		}
		break;

	case 3:
		for (int i = 0; i < theScene->entities.size(); i++)
		{
			if (theScene->entities[i]->meshName == "mig29_xyz.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("leaves.bmp", 1.0f));
			}
			else if (theScene->entities[i]->meshName == "Raider_ASCII_UVtex.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("lava.bmp", 1.0f));
			}
			else if (theScene->entities[i]->meshName == "Viper_MkVII_ASCII_UVTex.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("rust.bmp", 0.5f));
			}
			else if (theScene->entities[i]->meshName == "Sample_Ship.ply")
			{
				theScene->entities[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("sh3.bmp", 1.0f));
			}
		}

		for (int i = 0; i < theScene->terrain.size(); i++)
		{
			if (theScene->terrain[i]->meshName == "MeshLabTerrain_FLAT_xyz_n_uv.ply")
			{
				theScene->terrain[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("lava.bmp", 0.5f));
				//heightMap
				theScene->terrain[i]->heightMap = new sTextureBindBlendInfo("heightmap.bmp", 1.f);
			}
			else if (theScene->terrain[i]->meshName == "MeshLabTerrain_FLAT_xyz_n_uv1.ply") {
				theScene->terrain[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("TropicalSunnyDayUp2048.bmp", 0.5f));
				theScene->terrain[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("water-texture.bmp", 0.5f));
			}
			else if (theScene->terrain[i]->meshName == "SmoothSphere_Inverted_Normals_xyz_n.ply")
			{
				theScene->terrain[i]->vecMeshCubeMaps.push_back(sTextureBindBlendInfo("space", 1.0f));
				theScene->terrain[i]->bIsSkyBoxObject = true;
			}
		}

		for (int i = 0; i < theScene->powerUps.size(); i++)
		{
			if (theScene->powerUps[i]->meshName == "health_pack.ply")
			{
				theScene->powerUps[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("health.bmp", 1.0f));
			}
			if (theScene->powerUps[i]->meshName == "bullet.ply")
			{
				theScene->powerUps[i]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("gold.bmp", 1.0f));
			}
		}
		break;
	}
}

int cSceneManager::getCurrentLevel() {
	return this->currentLevel;
}

void cSceneManager::loadNextLevel(sScene* g_pCurrentScene, cPlayer* thePlayer) {
	if (this->currentLevel == this->numLevels) {
		this->currentLevel = 0;
	}
	else {
		this->currentLevel++;
	}

	//load the scene info
	sScene tempScene = this->getSceneById(this->currentLevel);
	this->copySceneFromCopyToPointer(tempScene, g_pCurrentScene);

	//find the player
	for (int i = 0; i < g_pCurrentScene->entities.size(); i++)
	{
		if (g_pCurrentScene->entities[i]->theEntity->getEntityType() == eEntityType::PLAYER_ENTITY) {
			thePlayer->thePlayerObject = g_pCurrentScene->entities[i];
		}
	}

	//populate the player data
	thePlayer->currentHealth = 100;
	thePlayer->rotationSpeed = 2.0f;
	thePlayer->projectiles.clear();
	thePlayer->projectilesToDraw.clear();
	//reset the projectiles
	for (int i = 0; i < thePlayer->projectilePool.size(); i++) {
		thePlayer->projectilePool[i].inUse = false;
	}
}

void cSceneManager::configurePowerUpsForScene(sScene* theScene, std::vector<cPowerUp*>& thePowerups) {
	thePowerups.clear();
	//set the modifiers
	for (int i = 0; i < theScene->powerUps.size(); i++) {
		if (theScene->powerUps[i]->meshName == "health_pack.ply") {
			thePowerups.push_back(new cPowerUp(eModifierType::MODIFIER_HEALTH, ePowerUpType::POWERUP_HEALTH, theScene->powerUps[i]));
		}
		else if (theScene->powerUps[i]->meshName == "bullet.ply") {
			thePowerups.push_back(new cPowerUp(eModifierType::MODIFIER_RANGE_INCREASE, ePowerUpType::POWERUP_RANGE_INCREASE, theScene->powerUps[i]));
		}
	}
}

void cSceneManager::setCurrentLevel(int levelIndex) {
	this->currentLevel = levelIndex;
}

void cSceneManager::createSceneRigidBodies(sScene* theScene, cVAOMeshManager* theMeshManager) {
	nPhysics::sRigidBodyDesc theDesc;
	nPhysics::iShape* theShape = NULL;
	nPhysics::eCollisionBodyType bodyType;

	//create the entity rigid bodies
	for (int i = 0; i < theScene->entities.size(); i++) {
		theShape = NULL;
		theDesc.Position = theScene->entities[i]->position;
		theDesc.Mass = theScene->entities[i]->scale*2.f;
		theDesc.Velocity = theScene->entities[i]->vel;
		theDesc.Rotation = theScene->entities[i]->orientation2;
		//populate the body desc info
		if (theScene->entities[i]->theEntity != NULL) {
			switch (theScene->entities[i]->theEntity->getEntityType())
			{
			case eEntityType::PLAYER_ENTITY:
				bodyType = nPhysics::eCollisionBodyType::PLAYER_BODY;
				break;
			case eEntityType::ENEMY_ENTITY:
				bodyType = nPhysics::eCollisionBodyType::ENEMY_BODY;
				break;
			case eEntityType::BOSS_ENTITY:
				break;
			}
		}
		else {
			bodyType = nPhysics::eCollisionBodyType::RIGID_BODY;
		}
		//get the rigid body info
		populateRigidBodyDescAndShape(*theScene->entities[i], theDesc, theShape, theMeshManager);
		//create the rigid body
		theScene->entities[i]->theBody = theFactory->CreateRigidBody(theDesc, theShape, bodyType);
		theWorld->AddCollisionBody(theScene->entities[i]->theBody);
	}

	//create the shield
	theScene->playerShield = new cShieldDescription(3, theScene->currentPlayer->thePlayerObject->position);
	//create the rigid bodies for the shield
	for (int i = 0; i < theScene->playerShield->pShieldBodies.size(); i++) {
		theShape = NULL;
		theDesc.Position = theScene->playerShield->pShieldBodies[i]->position;
		theDesc.Mass = theScene->playerShield->pShieldBodies[i]->scale*2.f;
		theDesc.Velocity = theScene->playerShield->pShieldBodies[i]->vel;
		theDesc.Rotation = theScene->playerShield->pShieldBodies[i]->orientation2;
		bodyType = nPhysics::eCollisionBodyType::SHIELD_BODY;

		//get the rigid body info
		populateRigidBodyDescAndShape(*theScene->playerShield->pShieldBodies[i], theDesc, theShape, theMeshManager);
		//create the rigid body
		theScene->playerShield->pShieldBodies[i]->theBody = theFactory->CreateRigidBody(theDesc, theShape, bodyType);
		theWorld->AddCollisionBody(theScene->playerShield->pShieldBodies[i]->theBody);
	}

	//create the power up rigid bodies
	for (int i = 0; i < theScene->powerUps.size(); i++) {
		theShape = NULL;
		theDesc.Position = theScene->powerUps[i]->position;
		theDesc.Mass = theScene->powerUps[i]->scale*2.f;
		theDesc.Velocity = theScene->powerUps[i]->vel;
		theDesc.Rotation = theScene->powerUps[i]->orientation2;
		//populate the body desc info
		if (theScene->powerUps[i]->typeOfPowerUp == ePowerUpType::POWERUP_HEALTH) {
			bodyType = nPhysics::eCollisionBodyType::ITEM_HEALTH_BODY;
		}
		else if (theScene->powerUps[i]->typeOfPowerUp == ePowerUpType::POWERUP_RANGE_INCREASE) {
			bodyType = nPhysics::eCollisionBodyType::ITEM_RANGE_BODY;
		}
		else {
			bodyType = nPhysics::eCollisionBodyType::RIGID_BODY;
		}
		//get the rigid body info
		populateRigidBodyDescAndShape(*theScene->powerUps[i], theDesc, theShape, theMeshManager);
		//create the rigid body
		theScene->powerUps[i]->theBody = theFactory->CreateRigidBody(theDesc, theShape, bodyType);
		theWorld->AddCollisionBody(theScene->powerUps[i]->theBody);
	}

	//create terrain rigid bodies
	for (int i = 0; i < theScene->terrain.size(); i++) {
		theShape = NULL;
		theDesc.Position = theScene->terrain[i]->position;
		theDesc.Mass = 0.f;
		theDesc.Velocity = theScene->terrain[i]->vel;
		theDesc.Rotation = theScene->terrain[i]->orientation2;
		//populate the body desc info
		bodyType = nPhysics::eCollisionBodyType::RIGID_BODY;
		//get the rigid body info
		if (theScene->terrain[i]->typeOfObject != eTypeOfObject::UNKNOWN) {
			populateRigidBodyDescAndShape(*theScene->terrain[i], theDesc, theShape, theMeshManager);
			//create the rigid body
			theScene->terrain[i]->theBody = theFactory->CreateRigidBody(theDesc, theShape, bodyType);
			theWorld->AddCollisionBody(theScene->terrain[i]->theBody);
		}
	}
}

void cSceneManager::populateRigidBodyDescAndShape(const cGameObject& object, nPhysics::sRigidBodyDesc& desc, nPhysics::iShape*& theShape, cVAOMeshManager* theMeshManager) {
	cMesh theMesh;
	theMeshManager->lookupMeshFromName(object.meshName, theMesh);
	float dot;
	float radius;
	float height;
	//create the shape and populate the desc based on type of object
	switch (object.typeOfObject) {
	case eTypeOfObject::SPHERE:
		theShape = theFactory->CreateSphere(object.scale / 3.f);
		desc.noCollisionResponse = true;
		break;
	case eTypeOfObject::PLANE:
		glm::vec3 normal(0.f, 1.f, 0.f);
		normal = glm::normalize(normal);
		desc.Mass = 0;
		dot = glm::dot(normal, object.position);
		desc.noCollisionResponse = false;
		theShape = theFactory->CreatePlane(normal, dot);
		break;
	case eTypeOfObject::BOX:
		theShape = theFactory->CreateBox(theMesh.maxExtentXYZ / 2.f);
		desc.noCollisionResponse = true;
		break;
	case eTypeOfObject::CYLINDER:
		glm::vec3 halfExtents(object.scale / 2.f);
		theShape = theFactory->CreateCylinder(halfExtents);
		desc.noCollisionResponse = true;
		break;
	case eTypeOfObject::CONE:
		radius = object.scale / 2.f;
		height = object.scale;
		theShape = theFactory->CreateCone(radius, height);
		desc.noCollisionResponse = false;
		break;
	}
}

void cSceneManager::loadConstraintsForScene(sScene* theScene) {
	//TODO: create the constraints for the player shield
	//hinge
	//requires 1 rigid body (the only box ont the right side of the screen)
	glm::vec3 yAxis = glm::vec3(0.f, 1.f, 0.f);
	for (int i = 0; i < theScene->playerShield->pShieldBodies.size(); i++) {
		glm::vec3 body1Loc = theScene->currentPlayer->thePlayerObject->position;
		glm::vec3 body2Loc = theScene->playerShield->pShieldBodies[i]->position;
		nPhysics::iConstraint* hinge = theFactory->CreateHingeConstraint(theScene->currentPlayer->thePlayerObject->theBody, body1Loc, yAxis, theScene->playerShield->pShieldBodies[i]->theBody, body2Loc, yAxis);
		hinge->EnableMotor(true);
		nPhysics::iRigidBody* tempRB = dynamic_cast<nPhysics::iRigidBody*>(theScene->playerShield->pShieldBodies[i]->theBody);
		tempRB->SetAngularVelocity(glm::vec3(1.f, 1.f, 1.f));
		tempRB->SetLinearFactor(glm::vec3(1.0f, 1.0f, 1.0f));
		hinge->EnableAngularMotor(true, 5.0f, 10.f);
		//hinge->SetMaxMotorImpulse(15.f);
		//hinge->SetMotorTargetVelocity(5.0f);
		theWorld->addConstraint(hinge);
	}
	//set the constraint limits
	//low , high, softness,bias, relaxation
	//hinge->SetLimits(glm::vec3(0.f,1.f,0.8f));

	//create cone
	cGameObject* coneObject;
	coneObject = new cGameObject();
	coneObject->meshName = "cone.ply";
	coneObject->scale = 3.f;
	coneObject->bIsWireFrame = true;
	coneObject->vecMehs2DTextures.push_back(sTextureBindBlendInfo("rust.bmp", 0.5f));
	coneObject->bIsSkyBoxObject = false;
	coneObject->position = glm::vec3(10.f, 0.0f, 30.f);

	nPhysics::sRigidBodyDesc theDesc;
	nPhysics::iShape* theShape = NULL;
	nPhysics::eCollisionBodyType bodyType = nPhysics::eCollisionBodyType::OBSTACLE_BODY;

	theDesc.Position = coneObject->position;
	theDesc.Mass = 1.f;
	theDesc.Velocity = glm::vec3(0.f);
	theDesc.Rotation = glm::vec3(0.f);
	theDesc.noCollisionResponse = false;
	theShape = theFactory->CreateCone(1.5f, 3.f);
	coneObject->theBody = theFactory->CreateCollisionBody(theDesc, theShape, bodyType);
	((nPhysics::iRigidBody*)coneObject->theBody)->SetLinearVelocity(glm::vec3(5.0f));
	((nPhysics::iRigidBody*)coneObject->theBody)->SetLinearFactor(glm::vec3(1.0f));
	theWorld->AddCollisionBody(coneObject->theBody);

	//cone2
	cGameObject* coneObject1;
	coneObject1 = new cGameObject();
	coneObject1->meshName = "cone.ply";
	coneObject1->scale = 1.f;
	coneObject1->position = glm::vec3(12.f, 0.0f, 30.f);
	coneObject1->bIsWireFrame = false;
	coneObject1->vecMehs2DTextures.push_back(sTextureBindBlendInfo("rust.bmp", 0.5f));
	coneObject1->bIsSkyBoxObject = false;

	nPhysics::sRigidBodyDesc theDesc1;
	nPhysics::iShape* theShape1 = NULL;
	nPhysics::eCollisionBodyType bodyType1 = nPhysics::eCollisionBodyType::OBSTACLE_BODY;

	theDesc1.Position = coneObject1->position;
	theDesc1.Mass = 1.f;
	theDesc1.Velocity = glm::vec3(0.f);
	theDesc1.Rotation = glm::vec3(0.f);
	theDesc1.noCollisionResponse = true;
	theShape1 = theFactory->CreateCone(0.5, 1.f);
	coneObject1->theBody = theFactory->CreateCollisionBody(theDesc1, theShape1, bodyType1);
	((nPhysics::iRigidBody*)coneObject1->theBody)->SetLinearFactor(glm::vec3(1.0f, 1.f, 1.0f));
	theWorld->AddCollisionBody(coneObject1->theBody);

	//cone3
	cGameObject* coneObject2;
	coneObject2 = new cGameObject();
	coneObject2->meshName = "cone.ply";
	coneObject2->scale = 1.f;
	coneObject2->position = glm::vec3(8.f, 0.0f, 30.f);
	coneObject2->bIsWireFrame = false;
	coneObject2->vecMehs2DTextures.push_back(sTextureBindBlendInfo("rust.bmp", 0.5f));
	coneObject2->bIsSkyBoxObject = false;

	nPhysics::sRigidBodyDesc theDesc2;
	nPhysics::iShape* theShape2 = NULL;
	nPhysics::eCollisionBodyType bodyType2 = nPhysics::eCollisionBodyType::OBSTACLE_BODY;

	theDesc2.Position = coneObject2->position;
	theDesc2.Mass = 1.f;
	theDesc2.Velocity = glm::vec3(0.f);
	theDesc2.Rotation = glm::vec3(0.f);
	theDesc2.noCollisionResponse = true;
	theShape2 = theFactory->CreateCone(1.5f, 3.f);
	coneObject2->theBody = theFactory->CreateCollisionBody(theDesc2, theShape2, bodyType2);
	((nPhysics::iRigidBody*)coneObject2->theBody)->SetLinearFactor(glm::vec3(1.0f, 0.f, 1.0f));
	theWorld->AddCollisionBody(coneObject2->theBody);

	//slider (the cone)
	nPhysics::iConstraint* slider = theFactory->CreateSliderConstraint(coneObject->theBody, coneObject->position, true);
	slider->SetLinearLimits(float(-20), float(20));
	slider->EnableMotor(true);
	slider->SetMotorTargetVelocity(30.f);
	theWorld->addConstraint(slider);

	//6 degrees of freedom (connecting the two cylinders together)
	nPhysics::iConstraint* sixDoF = theFactory->Create6DOFConstraint(coneObject1->theBody, coneObject->theBody, coneObject1->position, coneObject->position, true);
	//sixDoF->SetLinearLimits(glm::vec3(-3.f, -3.f, -3.f), glm::vec3(3.f, 3.f, 3.f));
	theWorld->addConstraint(sixDoF);

	//cone twist (one of the small spheres has this on it)
	nPhysics::iConstraint* pTwist = theFactory->CreateConeTwistConstraint(coneObject1->theBody, coneObject2->theBody);
	theWorld->addConstraint(pTwist);

	//create a slider with a 6dof and cone twist attached to it
	theScene->obstacles.push_back(coneObject);
	theScene->obstacles.push_back(coneObject1);
	theScene->obstacles.push_back(coneObject2);
}

void cSceneManager::loadAssimpLoadedModelsIntoScene(sScene* theScene) {
	//create the three models and add them to the scene
	float treeRotation = getRandInRange(0.f, 360.f);
	float treeXOrientation = getRandInRange(89.f, 90.5f);

	nPhysics::iShape* shape;
	shape = theFactory->CreateCylinder(glm::vec3(2.5f, 5.f, 2.5f));

	nPhysics::sRigidBodyDesc desc;
	glm::vec3 bodyPos;

	//tree 1
	cGameObject* toonTree = new cGameObject();
	toonTree->meshName = "assets/models/Cartoon_Tree.blend0";
	toonTree->scale = 2.f;
	toonTree->bIsWireFrame = false;
	toonTree->bIsSkyBoxObject = false;
	toonTree->bIsReflectRefract = false;
	toonTree->diffuseColour = glm::vec4(1.f);
	toonTree->diffuseColour = brown,
		toonTree->orientation2.y = treeRotation;
	toonTree->orientation2.x = 80.f;
	toonTree->isNoiseGenerating = false;
	toonTree->position = glm::vec3(-10.f, -2.f, 0.f);
	toonTree->theParticleEmitter->position = toonTree->position;
	toonTree->theParticleEmitter->position.y += 15;
	toonTree->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");

	toonTree->theParticleEmitter->init(NUMPARTICLES, 100,
		glm::vec3(-3.1f, -2.1f, -3.1f),	// Min init vel
		glm::vec3(1.1f, 0.1f, 1.1f),	// max init vel
		1.0f, 10.0f,
		glm::vec3(-3.0f, -3.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

	//create th rigid body for the tree trunk
	desc.Mass = 1.f;
	desc.noCollisionResponse = false;
	bodyPos = toonTree->position;
	bodyPos.y += 3.f;
	desc.Position = bodyPos;
	desc.Rotation = glm::vec3(0.f, 0.f, 0.f);
	toonTree->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::TREE_BODY);
	theWorld->AddCollisionBody(toonTree->theBody);
	theScene->terrain.push_back(toonTree);

	cGameObject* toonTree1 = new cGameObject();
	toonTree1->meshName = "assets/models/Cartoon_Tree.blend1";
	toonTree1->scale = 2.f;
	toonTree1->bIsWireFrame = false;
	toonTree1->bIsSkyBoxObject = false;
	toonTree1->bIsReflectRefract = false;
	toonTree1->isNoiseGenerating = false;
	toonTree1->orientation2.y = treeRotation;
	toonTree1->orientation2.x = 80.f;
	toonTree1->diffuseColour = green;
	toonTree1->position = glm::vec3(-10.f, -2.f, 0.f);
	theScene->terrain.push_back(toonTree1);

	//tree 2
	treeXOrientation = getRandInRange(89.f, 90.5f);
	treeRotation = getRandInRange(0.f, 360.f);
	cGameObject* toonTreeTrunk = new cGameObject();
	toonTreeTrunk->meshName = "assets/models/Cartoon_Tree.blend0";
	toonTreeTrunk->scale = 2.f;
	toonTreeTrunk->bIsWireFrame = false;
	toonTreeTrunk->bIsSkyBoxObject = false;
	toonTreeTrunk->bIsReflectRefract = false;
	toonTreeTrunk->diffuseColour = glm::vec4(1.f);
	toonTreeTrunk->diffuseColour = brown;
	toonTreeTrunk->orientation2.x = 80.f;
	toonTreeTrunk->orientation2.y = treeRotation;
	toonTreeTrunk->isNoiseGenerating = false;
	toonTreeTrunk->position = glm::vec3(-40.f, -2.f, 0.f);
	toonTreeTrunk->theParticleEmitter->position = toonTreeTrunk->position;
	toonTreeTrunk->theParticleEmitter->position.y += 15;
	toonTreeTrunk->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");

	toonTreeTrunk->theParticleEmitter->init(NUMPARTICLES, 100,
		glm::vec3(-3.1f, -2.1f, -3.1f),	// Min init vel
		glm::vec3(1.1f, 0.1f, 1.1f),	// max init vel
		1.0f, 10.0f,
		glm::vec3(-3.0f, -3.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

	desc.Mass = 1.f;
	desc.noCollisionResponse = false;
	bodyPos = toonTreeTrunk->position;
	bodyPos.y += 3.f;
	desc.Position = bodyPos;
	desc.Rotation = glm::vec3(0.f, 0.f, 0.f);
	toonTreeTrunk->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::TREE_BODY);
	theWorld->AddCollisionBody(toonTreeTrunk->theBody);
	theScene->terrain.push_back(toonTreeTrunk);

	theScene->terrain.push_back(toonTreeTrunk);

	cGameObject* toonTree2 = new cGameObject();
	toonTree2->meshName = "assets/models/Cartoon_Tree.blend1";
	toonTree2->scale = 2.f;
	toonTree2->bIsWireFrame = false;
	toonTree2->bIsSkyBoxObject = false;
	toonTree2->bIsReflectRefract = false;
	toonTree2->isNoiseGenerating = false;
	toonTree2->orientation2.x = 80.f;
	toonTree2->orientation2.y = treeRotation;
	toonTree2->diffuseColour = purple;
	toonTree2->position = glm::vec3(-40.f, -2.f, 0.f);
	theScene->terrain.push_back(toonTree2);

	//tree3
	treeXOrientation = getRandInRange(89.f, 90.5f);
	treeRotation = getRandInRange(0.f, 360.f);
	cGameObject* toonTreeTrunk1 = new cGameObject();
	toonTreeTrunk1->meshName = "assets/models/Cartoon_Tree.blend0";
	toonTreeTrunk1->scale = 2.f;
	toonTreeTrunk1->bIsWireFrame = false;
	toonTreeTrunk1->bIsSkyBoxObject = false;
	toonTreeTrunk1->bIsReflectRefract = false;
	toonTreeTrunk1->diffuseColour = glm::vec4(1.f);
	toonTreeTrunk1->diffuseColour = brown;
	toonTreeTrunk1->orientation2.y = treeRotation;
	toonTreeTrunk1->orientation2.x = 80.f;
	toonTreeTrunk1->isNoiseGenerating = false;
	toonTreeTrunk1->position = glm::vec3(-10.f, -2.f, 40.f);
	toonTreeTrunk1->theParticleEmitter->position = toonTreeTrunk1->position;
	toonTreeTrunk1->theParticleEmitter->position.y += 15;
	toonTreeTrunk1->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");

	toonTreeTrunk1->theParticleEmitter->init(NUMPARTICLES, 100,
		glm::vec3(-3.1f, -2.1f, -3.1f),	// Min init vel
		glm::vec3(1.1f, 0.1f, 1.1f),	// max init vel
		1.0f, 10.0f,
		glm::vec3(-3.0f, -3.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

	desc.Mass = 1.f;
	desc.noCollisionResponse = false;
	bodyPos = toonTreeTrunk1->position;
	bodyPos.y += 3.f;
	desc.Position = bodyPos;
	desc.Rotation = glm::vec3(0.f, 0.f, 0.f);
	toonTreeTrunk1->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::TREE_BODY);
	theWorld->AddCollisionBody(toonTreeTrunk1->theBody);
	theScene->terrain.push_back(toonTreeTrunk1);

	cGameObject* toonTree3 = new cGameObject();
	toonTree3->meshName = "assets/models/Cartoon_Tree.blend1";
	toonTree3->scale = 2.f;
	toonTree3->bIsWireFrame = false;
	toonTree3->bIsSkyBoxObject = false;
	toonTree3->bIsReflectRefract = false;
	toonTree3->isNoiseGenerating = false;
	toonTree3->orientation2.x = 80.f;
	toonTree3->orientation2.y = treeRotation;
	toonTree3->diffuseColour = orange;
	toonTree3->position = glm::vec3(-10.f, -2.f, 40.f);
	theScene->terrain.push_back(toonTree3);

	//tree4
	treeXOrientation = getRandInRange(89.f, 90.5f);
	treeRotation = getRandInRange(0.f, 360.f);
	cGameObject* toonTreeTrunk2 = new cGameObject();
	toonTreeTrunk2->meshName = "assets/models/Cartoon_Tree.blend0";
	toonTreeTrunk2->scale = 2.f;
	toonTreeTrunk2->bIsWireFrame = false;
	toonTreeTrunk2->bIsSkyBoxObject = false;
	toonTreeTrunk2->bIsReflectRefract = false;
	toonTreeTrunk2->diffuseColour = glm::vec4(1.f);
	toonTreeTrunk2->diffuseColour = brown;
	toonTreeTrunk2->orientation2.x = 80.f;
	toonTreeTrunk2->orientation2.y = treeRotation;
	toonTreeTrunk2->isNoiseGenerating = false;
	toonTreeTrunk2->position = glm::vec3(-40.f, -2.f, 40.f);
	toonTreeTrunk2->theParticleEmitter->position = toonTreeTrunk2->position;
	toonTreeTrunk2->theParticleEmitter->position.y += 15;
	toonTreeTrunk2->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");

	toonTreeTrunk2->theParticleEmitter->init(NUMPARTICLES, 100,
		glm::vec3(-3.1f, -2.1f, -3.1f),	// Min init vel
		glm::vec3(1.1f, 0.1f, 1.1f),	// max init vel
		1.0f, 10.0f,
		glm::vec3(-3.0f, -3.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

	desc.Mass = 1.f;
	desc.noCollisionResponse = false;
	bodyPos = toonTreeTrunk2->position;
	bodyPos.y += 3.f;
	desc.Position = bodyPos;
	desc.Rotation = glm::vec3(0.f, 0.f, 0.f);
	toonTreeTrunk2->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::TREE_BODY);
	theWorld->AddCollisionBody(toonTreeTrunk2->theBody);
	theScene->terrain.push_back(toonTreeTrunk2);

	cGameObject* toonTree4 = new cGameObject();
	toonTree4->meshName = "assets/models/Cartoon_Tree.blend1";
	toonTree4->scale = 2.f;
	toonTree4->bIsWireFrame = false;
	toonTree4->bIsSkyBoxObject = false;
	toonTree4->bIsReflectRefract = false;
	toonTree4->isNoiseGenerating = false;
	toonTree4->orientation2.x = 80.f;
	toonTree4->orientation2.y = treeRotation;
	toonTree4->diffuseColour = red;
	toonTree4->position = glm::vec3(-40.f, -2.f, 40.f);
	theScene->terrain.push_back(toonTree4);

	//tree 5
	treeXOrientation = getRandInRange(89.f, 90.5f);
	treeRotation = getRandInRange(0.f, 360.f);
	cGameObject* toonTreeTrunk3 = new cGameObject();
	toonTreeTrunk3->meshName = "assets/models/Cartoon_Tree.blend0";
	toonTreeTrunk3->scale = 2.f;
	toonTreeTrunk3->bIsWireFrame = false;
	toonTreeTrunk3->bIsSkyBoxObject = false;
	toonTreeTrunk3->bIsReflectRefract = false;
	toonTreeTrunk3->diffuseColour = glm::vec4(1.f);
	toonTreeTrunk3->diffuseColour = brown;
	toonTreeTrunk3->orientation2.y = treeRotation;
	toonTreeTrunk3->orientation2.x = 80.f;
	toonTreeTrunk3->isNoiseGenerating = false;
	toonTreeTrunk3->position = glm::vec3(-80.f, -2.f, 15.f);
	toonTreeTrunk3->theParticleEmitter->position = toonTreeTrunk3->position;
	toonTreeTrunk3->theParticleEmitter->position.y += 15;
	toonTreeTrunk3->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");

	toonTreeTrunk3->theParticleEmitter->init(NUMPARTICLES, 100,
		glm::vec3(-3.1f, -2.1f, -3.1f),	// Min init vel
		glm::vec3(1.1f, 0.1f, 1.1f),	// max init vel
		1.0f, 10.0f,
		glm::vec3(-3.0f, -3.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

	desc.Mass = 1.f;
	desc.noCollisionResponse = false;
	bodyPos = toonTreeTrunk3->position;
	bodyPos.y += 3.f;
	desc.Position = bodyPos;
	desc.Rotation = glm::vec3(0.f, 0.f, 0.f);
	toonTreeTrunk3->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::TREE_BODY);
	theWorld->AddCollisionBody(toonTreeTrunk3->theBody);
	theScene->terrain.push_back(toonTreeTrunk3);

	cGameObject* toonTree32 = new cGameObject();
	toonTree32->meshName = "assets/models/Cartoon_Tree.blend1";
	toonTree32->scale = 2.f;
	toonTree32->bIsWireFrame = false;
	toonTree32->bIsSkyBoxObject = false;
	toonTree32->bIsReflectRefract = false;
	toonTree32->isNoiseGenerating = false;
	toonTree32->orientation2.x = 80.f;
	toonTree32->orientation2.y = treeRotation;
	toonTree32->diffuseColour = green;
	toonTree32->position = glm::vec3(-80.f, -2.f, 15.f);
	theScene->terrain.push_back(toonTree32);

	//Tree 6
	treeXOrientation = getRandInRange(89.f, 90.5f);
	treeRotation = getRandInRange(0.f, 360.f);
	cGameObject* toonTreeTrunk4 = new cGameObject();
	toonTreeTrunk4->meshName = "assets/models/Cartoon_Tree.blend0";
	toonTreeTrunk4->scale = 2.f;
	toonTreeTrunk4->bIsWireFrame = false;
	toonTreeTrunk4->bIsSkyBoxObject = false;
	toonTreeTrunk4->bIsReflectRefract = false;
	toonTreeTrunk4->diffuseColour = glm::vec4(1.f);
	toonTreeTrunk4->diffuseColour = brown;
	toonTreeTrunk4->orientation2.y = getRandInRange(0.f, 360.f);
	toonTreeTrunk4->orientation2.x = 80.f;
	toonTreeTrunk4->orientation2.y = treeRotation;
	toonTreeTrunk4->isNoiseGenerating = false;
	toonTreeTrunk4->position = glm::vec3(-80.f, -2.f, 55.f);
	toonTreeTrunk4->theParticleEmitter->position = toonTreeTrunk4->position;
	toonTreeTrunk4->theParticleEmitter->position.y += 15;
	toonTreeTrunk4->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");

	toonTreeTrunk4->theParticleEmitter->init(NUMPARTICLES, 100,
		glm::vec3(-3.1f, -2.1f, -3.1f),	// Min init vel
		glm::vec3(1.1f, 0.1f, 1.1f),	// max init vel
		1.0f, 10.0f,
		glm::vec3(-3.0f, -3.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

	desc.Mass = 1.f;
	desc.noCollisionResponse = false;
	bodyPos = toonTreeTrunk4->position;
	bodyPos.y += 3.f;
	desc.Position = bodyPos;
	desc.Rotation = glm::vec3(0.f, 0.f, 0.f);
	toonTreeTrunk4->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::TREE_BODY);
	theWorld->AddCollisionBody(toonTreeTrunk4->theBody);
	theScene->terrain.push_back(toonTreeTrunk4);

	cGameObject* toonTree5 = new cGameObject();
	toonTree5->meshName = "assets/models/Cartoon_Tree.blend1";
	toonTree5->scale = 2.f;
	toonTree5->bIsWireFrame = false;
	toonTree5->bIsSkyBoxObject = false;
	toonTree5->bIsReflectRefract = false;
	toonTree5->isNoiseGenerating = false;
	toonTree5->orientation2.x = 80.f;
	toonTree5->orientation2.y = treeRotation;
	toonTree5->diffuseColour = purple;
	toonTree5->position = glm::vec3(-80.f, -2.f, 55.f);
	theScene->terrain.push_back(toonTree5);

	//Tree 7
	treeXOrientation = getRandInRange(89.f, 90.5f);
	treeRotation = getRandInRange(0.f, 360.f);
	cGameObject* toonTreeTrunk5 = new cGameObject();
	toonTreeTrunk5->meshName = "assets/models/Cartoon_Tree.blend0";
	toonTreeTrunk5->scale = 2.f;
	toonTreeTrunk5->bIsWireFrame = false;
	toonTreeTrunk5->bIsSkyBoxObject = false;
	toonTreeTrunk5->bIsReflectRefract = false;
	toonTreeTrunk5->diffuseColour = glm::vec4(1.f);
	toonTreeTrunk5->diffuseColour = brown;
	toonTreeTrunk5->orientation2.x = 80.f;
	toonTreeTrunk5->orientation2.y = treeRotation;
	toonTreeTrunk5->isNoiseGenerating = false;
	toonTreeTrunk5->position = glm::vec3(-80.f, -2.f, -55.f);
	toonTreeTrunk5->theParticleEmitter->position = toonTreeTrunk5->position;
	toonTreeTrunk5->theParticleEmitter->position.y += 15;
	toonTreeTrunk5->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");

	toonTreeTrunk5->theParticleEmitter->init(NUMPARTICLES, 100,
		glm::vec3(-3.1f, -2.1f, -3.1f),	// Min init vel
		glm::vec3(1.1f, 0.1f, 1.1f),	// max init vel
		1.0f, 10.0f,
		glm::vec3(-3.0f, -3.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

	desc.Mass = 1.f;
	desc.noCollisionResponse = false;
	bodyPos = toonTreeTrunk5->position;
	bodyPos.y += 3.f;
	desc.Position = bodyPos;
	desc.Rotation = glm::vec3(0.f, 0.f, 0.f);
	toonTreeTrunk5->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::TREE_BODY);
	theWorld->AddCollisionBody(toonTreeTrunk5->theBody);
	theScene->terrain.push_back(toonTreeTrunk5);

	cGameObject* toonTree6 = new cGameObject();
	toonTree6->meshName = "assets/models/Cartoon_Tree.blend1";
	toonTree6->scale = 2.f;
	toonTree6->bIsWireFrame = false;
	toonTree6->bIsSkyBoxObject = false;
	toonTree6->bIsReflectRefract = false;
	toonTree6->isNoiseGenerating = false;
	toonTree6->orientation2.x = 80.f;
	toonTree6->diffuseColour = orange;
	toonTree6->orientation2.y = treeRotation;
	toonTree6->position = glm::vec3(-80.f, -2.f, -55.f);
	theScene->terrain.push_back(toonTree6);

	//Tree 8
	treeXOrientation = getRandInRange(89.f, 90.5f);
	treeRotation = getRandInRange(0.f, 360.f);
	cGameObject* toonTreeTrunk6 = new cGameObject();
	toonTreeTrunk6->meshName = "assets/models/Cartoon_Tree.blend0";
	toonTreeTrunk6->scale = 2.f;
	toonTreeTrunk6->bIsWireFrame = false;
	toonTreeTrunk6->bIsSkyBoxObject = false;
	toonTreeTrunk6->bIsReflectRefract = false;
	toonTreeTrunk6->diffuseColour = glm::vec4(1.f);
	toonTreeTrunk6->diffuseColour = brown;
	toonTreeTrunk6->orientation2.x = 80.f;
	toonTreeTrunk6->orientation2.y = treeRotation;
	toonTreeTrunk6->isNoiseGenerating = false;
	toonTreeTrunk6->position = glm::vec3(-80.f, -2.f, -15.f);
	toonTreeTrunk6->theParticleEmitter->position = toonTreeTrunk6->position;
	toonTreeTrunk6->theParticleEmitter->position.y += 15;
	toonTreeTrunk6->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");

	toonTreeTrunk6->theParticleEmitter->init(NUMPARTICLES, 100,
		glm::vec3(-3.1f, -2.1f, -3.1f),	// Min init vel
		glm::vec3(1.1f, 0.1f, 1.1f),	// max init vel
		1.0f, 10.0f,
		glm::vec3(-3.0f, -3.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

	desc.Mass = 1.f;
	desc.noCollisionResponse = false;
	bodyPos = toonTreeTrunk6->position;
	bodyPos.y += 3.f;
	desc.Position = bodyPos;
	desc.Rotation = glm::vec3(0.f, 0.f, 0.f);
	toonTreeTrunk6->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::TREE_BODY);
	theWorld->AddCollisionBody(toonTreeTrunk6->theBody);
	theScene->terrain.push_back(toonTreeTrunk6);

	cGameObject* toonTree7 = new cGameObject();
	toonTree7->meshName = "assets/models/Cartoon_Tree.blend1";
	toonTree7->scale = 2.f;
	toonTree7->bIsWireFrame = false;
	toonTree7->bIsSkyBoxObject = false;
	toonTree7->bIsReflectRefract = false;
	toonTree7->isNoiseGenerating = false;
	toonTree7->orientation2.x = 80.f;
	toonTree7->orientation2.y = treeRotation;
	toonTree7->diffuseColour = red;
	toonTree7->position = glm::vec3(-80.f, -2.f, -15.f);
	theScene->terrain.push_back(toonTree7);

	//Tree 9
	treeXOrientation = getRandInRange(89.f, 90.5f);
	treeRotation = getRandInRange(0.f, 360.f);
	cGameObject* toonTreeTrunk7 = new cGameObject();
	toonTreeTrunk7->meshName = "assets/models/Cartoon_Tree.blend0";
	toonTreeTrunk7->scale = 2.f;
	toonTreeTrunk7->bIsWireFrame = false;
	toonTreeTrunk7->bIsSkyBoxObject = false;
	toonTreeTrunk7->bIsReflectRefract = false;
	toonTreeTrunk7->diffuseColour = glm::vec4(1.f);
	toonTreeTrunk7->diffuseColour = brown;
	toonTreeTrunk7->orientation2.x = 80.f;
	toonTreeTrunk7->orientation2.y = treeRotation;
	toonTreeTrunk7->isNoiseGenerating = false;
	toonTreeTrunk7->position = glm::vec3(-40.f, -2.f, -15.f);
	toonTreeTrunk7->theParticleEmitter->position = toonTreeTrunk7->position;
	toonTreeTrunk7->theParticleEmitter->position.y += 15;
	toonTreeTrunk7->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");

	toonTreeTrunk7->theParticleEmitter->init(NUMPARTICLES, 100,
		glm::vec3(-3.1f, -2.1f, -3.1f),	// Min init vel
		glm::vec3(1.1f, 0.1f, 1.1f),	// max init vel
		1.0f, 10.0f,
		glm::vec3(-3.0f, -3.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

	desc.Mass = 1.f;
	desc.noCollisionResponse = false;
	bodyPos = toonTreeTrunk7->position;
	bodyPos.y += 3.f;
	desc.Position = bodyPos;
	desc.Rotation = glm::vec3(0.f, 0.f, 0.f);
	toonTreeTrunk7->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::TREE_BODY);
	theWorld->AddCollisionBody(toonTreeTrunk7->theBody);
	theScene->terrain.push_back(toonTreeTrunk7);

	cGameObject* toonTree8 = new cGameObject();
	toonTree8->meshName = "assets/models/Cartoon_Tree.blend1";
	toonTree8->scale = 2.f;
	toonTree8->bIsWireFrame = false;
	toonTree8->bIsSkyBoxObject = false;
	toonTree8->bIsReflectRefract = false;
	toonTree8->isNoiseGenerating = false;
	toonTree8->orientation2.x = 80.f;
	toonTree8->orientation2.y = treeRotation;
	toonTree8->diffuseColour = green;
	toonTree8->position = glm::vec3(-40.f, -2.f, -15.f);
	theScene->terrain.push_back(toonTree8);

	//Tree 10
	treeXOrientation = getRandInRange(89.f, 90.5f);
	treeRotation = getRandInRange(0.f, 360.f);
	cGameObject* toonTreeTrunk8 = new cGameObject();
	toonTreeTrunk8->meshName = "assets/models/Cartoon_Tree.blend0";
	toonTreeTrunk8->scale = 2.f;
	toonTreeTrunk8->bIsWireFrame = false;
	toonTreeTrunk8->bIsSkyBoxObject = false;
	toonTreeTrunk8->bIsReflectRefract = false;
	toonTreeTrunk8->diffuseColour = glm::vec4(1.f);
	toonTreeTrunk8->diffuseColour = brown;
	toonTreeTrunk8->orientation2.x = 80.f;
	toonTreeTrunk8->orientation2.y = treeRotation;
	toonTreeTrunk8->isNoiseGenerating = false;
	toonTreeTrunk8->position = glm::vec3(0.f, -2.f, -15.f);
	toonTreeTrunk8->theParticleEmitter->position = toonTreeTrunk8->position;
	toonTreeTrunk8->theParticleEmitter->position.y += 15;
	toonTreeTrunk8->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");

	toonTreeTrunk8->theParticleEmitter->init(NUMPARTICLES, 100,
		glm::vec3(-3.1f, -2.1f, -3.1f),	// Min init vel
		glm::vec3(1.1f, 0.1f, 1.1f),	// max init vel
		1.0f, 10.0f,
		glm::vec3(-3.0f, -3.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

	desc.Mass = 1.f;
	desc.noCollisionResponse = false;
	bodyPos = toonTreeTrunk8->position;
	bodyPos.y += 3.f;
	desc.Position = bodyPos;
	desc.Rotation = glm::vec3(0.f, 0.f, 0.f);
	toonTreeTrunk8->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::TREE_BODY);
	theWorld->AddCollisionBody(toonTreeTrunk8->theBody);
	theScene->terrain.push_back(toonTreeTrunk8);

	cGameObject* toonTree9 = new cGameObject();
	toonTree9->meshName = "assets/models/Cartoon_Tree.blend1";
	toonTree9->scale = 2.f;
	toonTree9->bIsWireFrame = false;
	toonTree9->bIsSkyBoxObject = false;
	toonTree9->bIsReflectRefract = false;
	toonTree9->isNoiseGenerating = false;
	toonTree9->orientation2.x = 80.f;
	toonTree9->orientation2.y = treeRotation;
	toonTree9->diffuseColour = purple;
	toonTree9->position = glm::vec3(0.f, -2.f, -15.f);
	theScene->terrain.push_back(toonTree9);

	//Tree 11
	treeXOrientation = getRandInRange(89.f, 90.5f);
	treeRotation = getRandInRange(0.f, 360.f);
	cGameObject* toonTreeTrunk9 = new cGameObject();
	toonTreeTrunk9->meshName = "assets/models/Cartoon_Tree.blend0";
	toonTreeTrunk9->scale = 2.f;
	toonTreeTrunk9->bIsWireFrame = false;
	toonTreeTrunk9->bIsSkyBoxObject = false;
	toonTreeTrunk9->bIsReflectRefract = false;
	toonTreeTrunk9->diffuseColour = glm::vec4(1.f);
	toonTreeTrunk9->diffuseColour = brown;
	toonTreeTrunk9->orientation2.x = 80.f;
	toonTreeTrunk9->orientation2.y = treeRotation;
	toonTreeTrunk9->isNoiseGenerating = false;
	toonTreeTrunk9->position = glm::vec3(40.f, -2.f, -15.f);
	toonTreeTrunk9->theParticleEmitter->position = toonTreeTrunk9->position;
	toonTreeTrunk9->theParticleEmitter->position.y += 15;
	toonTreeTrunk9->theParticleEmitter->setParticleTextures("leaves.bmp", "rust.bmp");

	toonTreeTrunk9->theParticleEmitter->init(NUMPARTICLES, 100,
		glm::vec3(-3.1f, -2.1f, -3.1f),	// Min init vel
		glm::vec3(1.1f, 0.1f, 1.1f),	// max init vel
		1.0f, 10.0f,
		glm::vec3(-3.0f, -3.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), "Sphere_xyz_n_uv.ply");

	desc.Mass = 1.f;
	desc.noCollisionResponse = false;
	bodyPos = toonTreeTrunk9->position;
	bodyPos.y += 3.f;
	desc.Position = bodyPos;
	desc.Rotation = glm::vec3(0.f, 0.f, 0.f);
	toonTreeTrunk9->theBody = theFactory->CreateRigidBody(desc, shape, nPhysics::eCollisionBodyType::TREE_BODY);
	theWorld->AddCollisionBody(toonTreeTrunk9->theBody);
	theScene->terrain.push_back(toonTreeTrunk9);

	cGameObject* toonTree10 = new cGameObject();
	toonTree10->meshName = "assets/models/Cartoon_Tree.blend1";
	toonTree10->scale = 2.f;
	toonTree10->bIsWireFrame = false;
	toonTree10->bIsSkyBoxObject = false;
	toonTree10->bIsReflectRefract = false;
	toonTree10->isNoiseGenerating = false;
	toonTree10->orientation2.x = 80.f;
	toonTree10->orientation2.y = treeRotation;
	toonTree10->diffuseColour = orange;
	toonTree10->position = glm::vec3(40.f, -2.f, -15.f);
	theScene->terrain.push_back(toonTree10);

	//cGameObject* mill = new cGameObject();
	//mill->meshName = "assets/models/LowPolyMill.blend0";
	//mill->scale = 0.5f;
	//mill->bIsWireFrame = false;
	//mill->bIsSkyBoxObject = false;
	//mill->bIsReflectRefract = false;
	//mill->diffuseColour = glm::vec4(1.f);
	//mill->isNoiseGenerating = false;
	//mill->position = glm::vec3(-10.f, 5.f, 0.f);
	//mill->vecMeshNames.push_back("assets/models/LowPolyMill.blend0");
	//mill->vecMeshNames.push_back("assets/models/LowPolyMill.blend1");
	//mill->vecMeshNames.push_back("assets/models/LowPolyMill.blend2");
	//mill->vecMeshNames.push_back("assets/models/LowPolyMill.blend3");
	//mill->vecMeshNames.push_back("assets/models/LowPolyMill.blend4");
	//mill->vecMeshNames.push_back("assets/models/LowPolyMill.blend5");
	//theScene->terrain.push_back(mill);
}