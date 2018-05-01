#include "ModelUtilities.h"
#include "cVAOMeshManager.h"
#include "cMesh.h"
#include "cGameObject.h"
#include "cLightManager.h"
#include "CTextureManager.h"
#include <glm\vec3.hpp>
#include <glm\glm.hpp>
#include "globalOpenGL_GLFW.h"
#include <iPhysicsFactory.h>
#include "cVAOMeshManager.h"
#include "cPathingManager.h"
#include "assimp\cAssimpBasic.h"
#include "assimp\cSimpleAssimpSkinnedMeshLoader_OneMesh.h"
#include "cAnimationState.h"
#include "cAnimationManager.h"
#include "Utilities.h"
#include "cFollowState.h"
#include "cEnemy.h"
#include <iPhysicsFactory.h>
#include <iostream>
extern nPhysics::iPhysicsWorld* theWorld;
extern nPhysics::iPhysicsFactory* theFactory;
// Declared in globalStuff.h
cSimpleAssimpSkinnedMesh* g_pRPGSkinnedMesh = NULL;
cSimpleAssimpSkinnedMesh* g_pBotSkinnedMesh = NULL;

extern glm::vec3 g_cameraXYZ;
extern std::vector<cGameObject*> g_vecGameObjects;
extern std::vector<cGameObject*> g_vecAnimations;
extern cVAOMeshManager*	g_pVAOManager;

// Takes a file name, loads a mesh
bool LoadPlyFileIntoMesh(std::string filename, cMesh &theMesh) {
	// Load the vertices
	// c_str() changes a string to a "c style char* string"
	std::ifstream plyFile(filename.c_str());

	if (!plyFile.is_open()) {	// Didn't open file, so return
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
	for (int index = 0; index < theMesh.numberOfVertices; index++) {
		//end_header
		//-0.0312216 0.126304 0.00514924 0.850855 0.5
		float x, y, z;//, confidence, intensity;

		plyFile >> x;
		plyFile >> y;
		plyFile >> z;
		//		plyFile >> confidence;
		//		plyFile >> intensity;

		theMesh.pVertices[index].x = x;	// vertices[index].x = x;
		theMesh.pVertices[index].y = y;	// vertices[index].y = y;
		theMesh.pVertices[index].z = z;
		theMesh.pVertices[index].r = 1.0f;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].g = 1.0f;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].b = 1.0f;	// vertices[index].r = 1.0f;
	}

	// Load the triangle (or face) information, too
	for (int count = 0; count < theMesh.numberOfTriangles; count++) {
		// 3 164 94 98
		int discard = 0;
		plyFile >> discard;									// 3
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
	}

	//theMesh.CalculateNormals();

	return true;
}

// Takes a file name, loads a mesh
bool LoadPlyFileIntoMeshWithNormals(std::string filename, cMesh &theMesh) {
	// Load the vertices
	// c_str() changes a string to a "c style char* string"
	std::string file = "assets\\models\\" + filename;
	std::ifstream plyFile(file.c_str());

	if (!plyFile.is_open()) {	// Didn't open file, so return
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
	for (int index = 0; index < theMesh.numberOfVertices; index++) {
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
	for (int count = 0; count < theMesh.numberOfTriangles; count++) {
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

// Takes a file name, loads a mesh
bool LoadPlyFileIntoMeshWith_Normals_and_UV(std::string filename, cMesh &theMesh) {
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

	return true;
}

bool LoadAnimations(int shaderId) {
	std::stringstream ssError;
	bool bAllGood = true;

	//    ___  _    _                      _  __  __           _
	//   / __|| |__(_) _ _   _ _   ___  __| ||  \/  | ___  ___| |_
	//   \__ \| / /| || ' \ | ' \ / -_)/ _` || |\/| |/ -_)(_-<| ' \
	//   |___/|_\_\|_||_||_||_||_|\___|\__,_||_|  |_|\___|/__/|_||_|
	//
	::g_pRPGSkinnedMesh = new cSimpleAssimpSkinnedMesh();

	if (!::g_pRPGSkinnedMesh->LoadMeshFromFile("assets/modelsFBX/RPG-Character(FBX2013).FBX"))
	{
		std::cout << "Error: problem loading the skinned mesh" << std::endl;
	}
	// Now load another animation file...
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Attack-Kick-L1(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Walk(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Fall(FBX2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Roll-Backward(FBX2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Roll-Forward(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Roll-Left(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Roll-Right(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Jump(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Run(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Strafe-Right(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Strafe-Backward(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Strafe-Left(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Jump(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-GetHit-B1(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Attack-L3(FBX2013).FBX");

	::g_pRPGSkinnedMesh->friendlyName = "RPG-Character";

	cMesh* pTheMesh = ::g_pRPGSkinnedMesh->CreateMeshObjectFromCurrentModel();

	if (pTheMesh)
	{
		if (!g_pVAOManager->loadMeshIntoVAO(*pTheMesh, shaderId, false))
		{
			std::cout << "Could not load skinned mesh model into new VAO" << std::endl;
		}
	}
	else
	{
		std::cout << "Could not create a cMesh object from skinned mesh file" << std::endl;
	}

	delete pTheMesh;

	// Skinned mesh  model
	cGameObject* pTempGO = new cGameObject();
	pTempGO->friendlyName = "Sophie";
	// Add a default animation
	pTempGO->pAnimationManager = new cAnimationManager();
	pTempGO->pAnimationManager->pAniState = new cAnimationState();
	pTempGO->pAnimationManager->pSimpleSkinnedMesh = new cSimpleAssimpSkinnedMesh();
	pTempGO->pAnimationManager->pSimpleSkinnedMesh = ::g_pRPGSkinnedMesh;
	pTempGO->pAnimationManager->pAniState = new cAnimationState();
	pTempGO->pAnimationManager->pAniState->defaultAnimation.name = "assets/modelsFBX/RPG-Character_Unarmed-Attack-L3(FBX2013).FBX";
	pTempGO->pAnimationManager->pAniState->defaultAnimation.frameStepTime = 0.0f;
	// Get the total time of the entire animation
	pTempGO->pAnimationManager->pAniState->defaultAnimation.totalTime = 0.4f;
	pTempGO->pAnimationManager->pAniState->defaultAnimation.currentTime = 0.4f;

	{
		cAnimationState::sStateDetails walkAnimation;
		walkAnimation.name = "assets/modelsFBX/RPG-Character_Unarmed-Walk(FBX2013).FBX";
		walkAnimation.friendlyName = "walk";
		//leftAnimation.totalTime = pTempGO->pSimpleSkinnedMesh->FindAnimationTotalTime(leftAnimation.name);
		walkAnimation.totalTime = pTempGO->pAnimationManager->pSimpleSkinnedMesh->GetDuration() - 0.1f;
		walkAnimation.frameStepTime = 0.01f;
		walkAnimation.currentTime = 0.05;
		pTempGO->pAnimationManager->animationStateMap[walkAnimation.friendlyName] = std::pair<bool, cAnimationState::sStateDetails>(false, walkAnimation);

		//RPG-Character_Unarmed-Attack-R3(FBX2013).FBX
		cAnimationState::sStateDetails rightPunch;
		rightPunch.name = "assets/modelsFBX/RPG-Character_Unarmed-Attack-R3(FBX2013).FBX";
		rightPunch.friendlyName = "rightPunch";
		//leftAnimation.totalTime = pTempGO->pSimpleSkinnedMesh->FindAnimationTotalTime(leftAnimation.name);
		rightPunch.totalTime = pTempGO->pAnimationManager->pSimpleSkinnedMesh->GetDuration();
		rightPunch.frameStepTime = 0.01f;
		rightPunch.currentTime = 0.05;
		pTempGO->pAnimationManager->animationStateMap[rightPunch.friendlyName] = std::pair<bool, cAnimationState::sStateDetails>(false, rightPunch);

		//RPG-Character_Unarmed-GetHit-B1(FBX2013).FBX
		cAnimationState::sStateDetails getHitAnimation;
		getHitAnimation.name = "RPG-Character_Unarmed-Stunned(FBX2013).FBX";
		getHitAnimation.friendlyName = "stunned";
		getHitAnimation.totalTime = pTempGO->pAnimationManager->pSimpleSkinnedMesh->FindAnimationTotalTime(getHitAnimation.name);
		getHitAnimation.totalTime = pTempGO->pAnimationManager->pSimpleSkinnedMesh->GetDuration();
		getHitAnimation.frameStepTime = 0.01f;
		pTempGO->pAnimationManager->animationStateMap[getHitAnimation.friendlyName] = std::pair<bool, cAnimationState::sStateDetails>(false, getHitAnimation);

		cAnimationState::sStateDetails attackAnimation;
		attackAnimation.name = "assets/modelsFBX/RPG-Character_Unarmed-Attack-L3(FBX2013).FBX";
		attackAnimation.friendlyName = "leftPunch";
		attackAnimation.totalTime = pTempGO->pAnimationManager->pSimpleSkinnedMesh->FindAnimationTotalTime(attackAnimation.name);
		attackAnimation.totalTime = pTempGO->pAnimationManager->pSimpleSkinnedMesh->GetDuration();
		attackAnimation.frameStepTime = 0.01f;
		pTempGO->pAnimationManager->animationStateMap[attackAnimation.friendlyName] = std::pair<bool, cAnimationState::sStateDetails>(false, attackAnimation);

		cAnimationState::sStateDetails deathAnimation;
		deathAnimation.name = "assets/modelsFBX/RPG-Character_Unarmed-Death1(FBX2013).FBX";
		deathAnimation.friendlyName = "death";
		deathAnimation.totalTime = pTempGO->pAnimationManager->pSimpleSkinnedMesh->FindAnimationTotalTime(deathAnimation.name);
		deathAnimation.totalTime = pTempGO->pAnimationManager->pSimpleSkinnedMesh->GetDuration();
		deathAnimation.frameStepTime = 0.01f;
		pTempGO->pAnimationManager->animationStateMap[deathAnimation.friendlyName] = std::pair<bool, cAnimationState::sStateDetails>(false, deathAnimation);
	}

	pTempGO->position = glm::vec3(10.0f, 3.7f, 0.0f);
	pTempGO->orientation2 = glm::vec3(0.0f, 0.0f, 0.0f);
	pTempGO->diffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	pTempGO->bIsWireFrame = true;
	pTempGO->meshName = "assets/modelsFBX/RPG-Character(FBX2013).FBX";
	pTempGO->scale = 0.03f;
	pTempGO->bHasAABB = false;
	pTempGO->typeOfObject = eTypeOfObject::UNKNOWN;
	pTempGO->theAIState = new cFollowState();
	pTempGO->theEntity = new cEnemy();
	pTempGO->theEntity->setSpeed(10.f);
	pTempGO->theEntity->setMaxHealth(200.f);

	nPhysics::sRigidBodyDesc theDesc;
	nPhysics::iShape* theShape = NULL;
	nPhysics::eCollisionBodyType bodyType = nPhysics::eCollisionBodyType::BOSS_BODY;

	theDesc.Position = pTempGO->position;
	theDesc.Mass = 1.f;
	theDesc.Velocity = glm::vec3(0.f);
	theDesc.Rotation = glm::vec3(0.f);
	theDesc.noCollisionResponse = true;
	//cylinder shape for the body
	cMesh tempMesh;
	g_pVAOManager->lookupMeshFromName(pTempGO->meshName, tempMesh);
	glm::vec3 extents = glm::vec3(1.f, 2.f, 1.f);
	theShape = theFactory->CreateCylinder(extents);

	pTempGO->theBody = theFactory->CreateRigidBody(theDesc, theShape, bodyType);
	theWorld->AddCollisionBody(pTempGO->theBody);

	::g_vecGameObjects.push_back(pTempGO);
	::g_vecAnimations.push_back(pTempGO);
	//load all the rigid bodies into the world
	cMesh theMesh;
	g_pVAOManager->lookupMeshFromName(pTempGO->meshName, theMesh);
	pTempGO->meshExtents = theMesh.maxExtentXYZ;

	//load all the files that aren't ply
	cAssimpBasic myAssimpLoader;
	cMesh rockTestMesh;
	std::string error;
	if (!myAssimpLoader.loadAllMeshesFromModel("assets/models/Stone_Stele.FBX", rockTestMesh, error, shaderId)) {
		std::cout << error << std::endl;
	}
	if (!g_pVAOManager->loadMeshIntoVAO(rockTestMesh, shaderId, false))
	{
		std::cout << "Could not load rock test mesh model into new VAO" << std::endl;
	}

	cMesh cartoonTree;
	if (!myAssimpLoader.loadAllMeshesFromModel("assets/models/Cartoon_Tree.blend", cartoonTree, error, shaderId)) {
		std::cout << error << std::endl;
	}
	if (!g_pVAOManager->loadMeshIntoVAO(cartoonTree, shaderId, false))
	{
		std::cout << "Could not load Cartoon_Tree mesh model into new VAO" << std::endl;
	}

	cMesh mill;
	if (!myAssimpLoader.loadAllMeshesFromModel("assets/models/LowPolyMill.blend", mill, error, shaderId)) {
		std::cout << error << std::endl;
	}
	if (!g_pVAOManager->loadMeshIntoVAO(mill, shaderId, false))
	{
		std::cout << "Could not load Cartoon_Tree mesh model into new VAO" << std::endl;
	}

	//CARTOON_HOUSE.max
	cMesh house;
	if (!myAssimpLoader.loadAllMeshesFromModel("assets/models/Enviroment_GroupTrees.obj", house, error, shaderId)) {
		std::cout << error << std::endl;
	}
	if (!g_pVAOManager->loadMeshIntoVAO(house, shaderId, false))
	{
		std::cout << "Could not load Cartoon_Tree mesh model into new VAO" << std::endl;
	}

	//cGameObject* stone = new cGameObject();
	//cGameObject* mill = new cGameObject();
	return true;
}

bool Load3DModelsIntoMeshManager(int shaderID, cVAOMeshManager* pVAOManager, std::string &error) {
	std::stringstream ssError;
	bool bAllGood = true;

	// *******************************************************
	// Test the assimp loader before the "old" loader
	cAssimpBasic myAssimpLoader;
	cMesh shipTestMesh;

	//    ___  _    _                      _  __  __           _
	//   / __|| |__(_) _ _   _ _   ___  __| ||  \/  | ___  ___| |_
	//   \__ \| / /| || ' \ | ' \ / -_)/ _` || |\/| |/ -_)(_-<| ' \
	//   |___/|_\_\|_||_||_||_||_|\___|\__,_||_|  |_|\___|/__/|_||_|
	//
	::g_pRPGSkinnedMesh = new cSimpleAssimpSkinnedMesh();

	if (!::g_pRPGSkinnedMesh->LoadMeshFromFile("assets/modelsFBX/RPG-Character(FBX2013).FBX")) {
		std::cout << "Error: problem loading the skinned mesh" << std::endl;
	}
	// Now load another animation file...
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Attack-Kick-L1(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Fall(FBX2013).fbx");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Death1(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Attack-L3(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Attack-R3(FBX2013).FBX");
	::g_pRPGSkinnedMesh->LoadMeshAnimation("assets/modelsFBX/RPG-Character_Unarmed-Stunned(FBX2013).FBX");

	cMesh* pTheMesh = ::g_pRPGSkinnedMesh->CreateMeshObjectFromCurrentModel();

	if (pTheMesh) {
		if (!g_pVAOManager->loadMeshIntoVAO(*pTheMesh, shaderID, false)) {
			std::cout << "Could not load skinned mesh model into new VAO" << std::endl;
		}
	}
	else {
		std::cout << "Could not create a cMesh object from skinned mesh file" << std::endl;
	}

	// Delete temporary mesh if still around
	if (pTheMesh) {
		delete pTheMesh;
	}

	if (!bAllGood) {
		// Copy the error string stream into the error string that
		//	gets "returned" (through pass by reference)
		error = ssError.str();
	}

	return bAllGood;
}

bool LoadSceneFromFileWithNormalAndUV(std::string& fileName) {
	//open the file
	std::ifstream modelAndSceneFile(fileName.c_str());
	int numModels = 0;
	int numModelsUV = 0;
	if (!modelAndSceneFile.is_open())
	{	// Didn't open file, so return
		return false;
	}
	ReadFileToToken(modelAndSceneFile, "CAMERA_POSITION");
	modelAndSceneFile >> g_cameraXYZ.x;
	modelAndSceneFile >> g_cameraXYZ.y;
	modelAndSceneFile >> g_cameraXYZ.z;

	//models with normal and uv
	ReadFileToToken(modelAndSceneFile, "NUMBER_OF_MODELS_WITH_UV");
	modelAndSceneFile >> numModelsUV;
	ReadFileToToken(modelAndSceneFile, "MODELS_WITH_UV_FOR_DRAWING");
	cGameObject* pTempGO;
	cPathingNode* pTempNode;

	int sphereCount = 0;
	for (int i = 0; i < numModelsUV; i++)
	{
		std::string tempString = "";
		pTempGO = new cGameObject();
		pTempNode = new cPathingNode();
		float tempFloat = 0.0f;
		float mass = 0.0f;
		modelAndSceneFile >> pTempGO->meshName;
		modelAndSceneFile >> pTempGO->position.x;
		modelAndSceneFile >> pTempGO->position.y;
		modelAndSceneFile >> pTempGO->position.z;
		modelAndSceneFile >> pTempGO->scale;
		modelAndSceneFile >> pTempGO->vel.x;
		modelAndSceneFile >> pTempGO->vel.y;
		modelAndSceneFile >> pTempGO->vel.z;
		modelAndSceneFile >> pTempGO->radius;
		modelAndSceneFile >> mass;
		modelAndSceneFile >> pTempGO->diffuseColour.x;
		modelAndSceneFile >> pTempGO->diffuseColour.y;
		modelAndSceneFile >> pTempGO->diffuseColour.z;
		modelAndSceneFile >> pTempGO->diffuseColour.a;
		modelAndSceneFile >> pTempGO->bIsWireFrame;
		modelAndSceneFile >> pTempGO->reflectRatio;
		modelAndSceneFile >> pTempGO->refractRatio;
		modelAndSceneFile >> tempFloat;
		pTempGO->orientation2.x = glm::radians(tempFloat);
		modelAndSceneFile >> tempFloat;
		pTempGO->orientation2.y = glm::radians(tempFloat);
		modelAndSceneFile >> tempFloat;
		pTempGO->orientation2.z = glm::radians(tempFloat);
		modelAndSceneFile >> tempString;
		modelAndSceneFile >> pTempGO->bHasAABB;
		modelAndSceneFile >> pTempGO->bIsReflectRefract;
		//modelAndSceneFile >> pTempNode->isActive;
		//modelAndSceneFile >> pTempNode->value;
		//set the position of the node
		pTempNode->position = pTempGO->position;
		//adjusting the y value so that when objects move
		//to the node it wont be inside the nodes object
		pTempNode->position.y += 2.0f;

		if (tempString == "UNKNOWN")
		{
			pTempGO->typeOfObject = eTypeOfObject::UNKNOWN;
		}
		else if (tempString == "SPHERE")
		{
			pTempGO->typeOfObject = eTypeOfObject::SPHERE;
		}
		else
		{
			pTempGO->typeOfObject = eTypeOfObject::PLANE;
		}

		//to store the node by
		pTempNode->nodeId = i;
		pTempGO->pathingNode = pTempNode;

		g_vecGameObjects.push_back(pTempGO);
	}

	//close the file stream
	modelAndSceneFile.close();
	return true;
}