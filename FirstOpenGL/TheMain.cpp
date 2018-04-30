#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>			// C++ cin, cout, etc.
//#include "linmath.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>		
#include <string>

#include <vector>	
#include "Utilities.h"
#include "cSceneManager.h"
#include "ModelUtilities.h"
#include "cMesh.h"
#include "cShader.h"
#include "cShaderManager.h" 

#include "cGameObject.h"
#include "cVAOMeshManager.h"

#include "cLightManager.h"
#include "Keyboard.h"
#include "DebugRenderer.h"

#include"CommandManager.h"
#include "OrientToTime.h"
#include "CTextureManager.h"
#include "RenderUtilities.h"
#include <iPhysicsFactory.h>
#include <enums.h>
#include "cTextRenderer.h"
#include "cFBO.h" 
#include "cSceneManager.h"
#include "cParticleManager.h"
#include "cSoundManager.h"
#include "cPlayer.h"
#include "Utilities.h"
#include "cCollisionHandler.h"
#include "cShieldDescription.h"
#include "cStateManager.h"
#include "GamePad.h"

int height = 480;	/* default */
int width = 640;	// default
bool KeysPressed[Keyboard::KeyCount];
void printError(const std::string& error);
std::string modelAndLightFile = "modelsandscene.txt";

cVAOMeshManager*	g_pVAOManager = 0;		// or NULL, or nullptr
cShaderManager*		g_pShaderManager;		// Heap, new (and delete)
cLightManager*		g_pLightManager;
cMesh*				g_GameTerrain;
DebugRenderer*		g_pTheDebugrender;
CTextureManager*	g_pTextureManager;
cSceneManager*		g_pSceneManager;
sScene*				g_pCurrentScene;
cParticleManager*   g_pParticleManager;
cSoundManager*		g_pSoundManager;
cCollisionHandler*	g_pCollisionHandler;
cStateManager*      g_pStateManager;
GamePad*			g_pGamePad;
cGameObject			tempObject;

std::vector<cGameObject*>  g_vecGameObjects;
std::vector<cGameObject*>  g_vecAnimations;

//text renderer
cTextRenderer*			g_ptheTextRenderer;
int						g_languageNum = 6;

//physics
nPhysics::iPhysicsWorld* theWorld;
nPhysics::iPhysicsFactory* theFactory;

GLint					sexyShaderID;

glm::vec3				g_cameraXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3				g_cameraTarget_XYZ = glm::vec3(18.0f, 0.0f, -5.0f);
glm::vec3				g_mainLightPosition = glm::vec3(0.f);
glm::vec3				g_cameraBackupPosition;

glm::mat4				g_lightSpaceMatrix;
glm::mat4				g_lightProjection;
glm::mat4				g_lightView;
int						g_GameObjectIndex = 0;
cGameObject *			g_curGameObject;
cGameObject*			g_pDebugObject;
double					g_lastTimeStep = 0.0;
bool					g_shakeCamera;
//for the movement of the teapot
CommandManager*			theParallelCommands;
CommandManager*			theSerialCommands = new CommandManager(false);

cFBO* g_pFBO;
cFBO* g_pFBOGeometry;

bool drawDebugInfo = true;
GLFWwindow* g_pGLFWWindow = NULL;
unsigned int SHADOW_WIDTH = 1920, SHADOW_HEIGHT = 1080;

void shakeCamera(float deltaTime);
const float TimeToShake = 3.f;
float currentShakeTime = 0.f;
bool bUseFog = true;
double deltaTime;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char** argv)
{
	//for rigid body debugging
	cGameObject debugObj;
	debugObj.meshName = "Sphere_xyz_n_uv.ply";
	debugObj.position = glm::vec3(0.f,0.f,0.f);
	debugObj.bIsWireFrame = true;
	debugObj.bIsSkyBoxObject = false;
	debugObj.diffuseColour = glm::vec4(1.f);
	debugObj.scale = 2.5f;
	debugObj.radius = 0.125f;

	//for rigid body debugging
	cGameObject debugCube;
	debugCube.meshName = "cone.ply";
	debugCube.position = glm::vec3(0.f, 0.f, 0.f);
	debugCube.bIsWireFrame = true;
	debugCube.bIsSkyBoxObject = false;
	debugCube.diffuseColour = glm::vec4(1.f);
	debugCube.scale = 1.5f;
	debugCube.radius = 0.125f;

	//for rigid body debugging
	cGameObject debugCylinder;
	debugCylinder.meshName = "cylinder.ply";
	debugCylinder.position = glm::vec3(0.f, 0.f, 0.f);
	debugCylinder.bIsWireFrame = true;
	debugCylinder.bIsSkyBoxObject = false;
	debugCylinder.diffuseColour = glm::vec4(1.f);
	debugCylinder.scale = 1.5f;
	debugCylinder.radius = 0.125f;

	g_curGameObject = new cGameObject();
	glfwSetErrorCallback(error_callback);
	InitPhysics();
	theWorld = theFactory->CreateWorld();

 	if (!glfwInit())
		exit(EXIT_FAILURE);

	std::string title = "OpenGL Rocks";

	loadConfigFile(width, height, title);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// C++ string
	// C no strings. Sorry. char    char name[7] = "Michael\0";
	g_pGLFWWindow = glfwCreateWindow(width, height,
		title.c_str(),
		NULL, NULL);

	if (!g_pGLFWWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//static key_callback is from Keyboard.h
	glfwSetKeyCallback(g_pGLFWWindow, Keyboard::key_callback);
	glfwMakeContextCurrent(g_pGLFWWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	std::cout << glGetString(GL_VENDOR) << " "
		<< glGetString(GL_RENDERER) << ", "
		<< glGetString(GL_VERSION) << std::endl;
	std::cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	::g_pShaderManager = new cShaderManager();
	cShader vertShader;
	cShader fragShader;

	vertShader.fileName = "simpleVert.glsl";
	fragShader.fileName = "simpleFrag.glsl";
	::g_pShaderManager->setBasePath("assets//shaders//");

	// Shader objects are passed by reference so that
	//	we can look at the results if we wanted to. 
	if (!::g_pShaderManager->createProgramFromFile(
		"mySexyShader", vertShader, fragShader))
	{
		std::cout << "Oh no! All is lost!!! Blame Loki!!!" << std::endl;
		std::cout << ::g_pShaderManager->getLastError() << std::endl;
		// Should we exit?? 
		return -1;
	}


	/////////////////////////////////////////////
	//MULTIPLE FRAME BUFFERS FOR BLOOM AND BLUR//
	/////////////////////////////////////////////
	::g_pVAOManager = new cVAOMeshManager();
	sexyShaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");

	std::string error;

	//Load models 
	if (g_pSceneManager->loadModelsFromModelInfoFile(std::string("assets/GameInfoFiles/ModelInfo.txt"), g_pVAOManager, sexyShaderID) == false) {
		std::cout << "Failed Loading Files Into Mesh Manager!" << std::endl;
		std::cout << ::g_pShaderManager->getLastError() << std::endl;
		// Should we exit?? 
		return -1;
	}
	//load the animations
	Load3DModelsIntoMeshManager(sexyShaderID, g_pVAOManager, error);

	//scene manager
	g_pSceneManager = new cSceneManager();
	g_pCurrentScene = new sScene();

	//read in the sccenes
	g_pSceneManager->loadSceneFromFileIntoSceneMap(std::string("assets/GameInfoFiles/Scene1.txt"), 0, g_pVAOManager);
	//g_pSceneManager->LoadSceneFromFileIntoSceneMap(std::string("assets/GameInfoFiles/Scene2.txt"), 1, g_pVAOManager);
	//g_pSceneManager->LoadSceneFromFileIntoSceneMap(std::string("assets/GameInfoFiles/Scene3.txt"), 2, g_pVAOManager);
	//g_pSceneManager->LoadSceneFromFileIntoSceneMap(std::string("assets/GameInfoFiles/Scene4.txt"), 3, g_pVAOManager);

	sScene tempScene = g_pSceneManager->getSceneById(0);
	//g_pSceneManager->copySceneFromCopyToPointer(tempScene, g_pCurrentScene);
	*g_pCurrentScene = tempScene;
	g_pCurrentScene->setPlayerPointer();
	g_pSceneManager->configurePowerUpsForScene(g_pCurrentScene,g_pCurrentScene->scenePowerUps);

	//load the animated character
	LoadAnimations(sexyShaderID);
	g_pCurrentScene->currentBoss = g_vecGameObjects[0];
	//set the physics listener
	g_pCollisionHandler = new cCollisionHandler();
	//g_pCollisionHandler->setCurrentScene(g_pCurrentScene);
	theWorld->SetCollisionListener(g_pCollisionHandler);

	////////////////////////
	////PARTICLE MANAGER///
	//////////////////////
	//create the particle manager
	//TODO:: hook this up again
	g_pParticleManager = new cParticleManager();
	//create enough emitters for each enemy
	//g_pParticleManager->createEmitters(g_vecEnemies.size());
	//g_pParticleManager->connectEmittersWithEntities(g_vecEnemies);


	/////////////////////
	////SOUND MANAGER///
	////////////////////
	g_pSoundManager = new cSoundManager();
	g_pSoundManager->initSoundManager();
	g_pSoundManager->readSoundsFromSoundFile(std::string("assets/GameInfoFiles/SoundInfo.txt"));
	//g_pSoundManager->setInitialBackgroundSound();

	  /////////////////////
	 //	Debug Renderer	//
	/////////////////////
	::g_pTheDebugrender = new DebugRenderer();
	if(!::g_pTheDebugrender->initialize(error))
	{
		printError(error);
	}

	  /////////////////////
	 //	Text Renderer	//
	/////////////////////
	g_ptheTextRenderer = new cTextRenderer();
	if (!g_ptheTextRenderer->initializeTextRenderer(error))
	{
		printError(error);
	}

	  /////////////////
	 //	Ligh Loader	//
	/////////////////
	GLint currentProgID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");
	//getUniformLocations(mvp_location, currentProgID);

	//load lights
	::g_pLightManager = new cLightManager();
	::g_pLightManager->CreateLights(1);
	::g_pLightManager->LoadShaderUniformLocations(currentProgID);
	if(!LoadLightsAttributesFromFile(modelAndLightFile, ::g_pLightManager->vecLights))
	{
		error = "Lights not loaded properly! Line 260:main";
		printError(error);
	}
	//set the position for the shadow mapping
	g_mainLightPosition = ::g_pLightManager->vecLights[0].position;
	  ////////////////////
	 //	Texture Loader //
	////////////////////

	//do the texture loading
	g_pTextureManager = new CTextureManager();
	g_pTextureManager->setBasePath("assets//textures");

	LoadTexturesFromFile(std::string("assets/GameInfoFiles/TextureInfo.txt"));

	//::g_pTextureManager->setBasePath("assets/textures/skybox");
	//if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
	//	"space",
	//	"SpaceBox_right1_posX.bmp",
	//	"SpaceBox_left2_negX.bmp",
	//	"SpaceBox_top3_posY.bmp",
	//	"SpaceBox_bottom4_negY.bmp",
	//	"SpaceBox_front5_posZ.bmp",
	//	"SpaceBox_back6_negZ.bmp", true, true))
	//{
	//	std::cout << "Didn't load skybox" << std::endl;
	//}

	::g_pTextureManager->setBasePath("assets/textures/skybox");
	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
		"sunny",
		"TropicalSunnyDayLeft2048.bmp",
		"TropicalSunnyDayRight2048.bmp",
		"TropicalSunnyDayDown2048.bmp",
		"TropicalSunnyDayUp2048.bmp",
		"TropicalSunnyDayFront2048.bmp",
		"TropicalSunnyDayBack2048.bmp", true, true))
	{
		std::cout << "Didn't load skybox" << std::endl;
	}

	//load the textures in the scene
	g_pSceneManager->loadLevelTextures(g_pCurrentScene);


	glEnable(GL_DEPTH);
	glCullFace(GL_BACK);
	// Gets the "current" time "tick" or "step"
	g_lastTimeStep = glfwGetTime();

	g_pFBO = new cFBO();

	//deferred rendering buffer
	if (!g_pFBO->init(width, 1060, error))
	{
		std::cout << "FBO error: " << error << std::endl;
	}
	else
	{
		std::cout << "FBO is good." << std::endl;
		std::cout << "\tFBO ID = " << g_pFBO->ID << std::endl;
		std::cout << "\tcolour texture ID = " << g_pFBO->colourTexture_0_ID << std::endl;
		std::cout << "\tnormal texture ID = " << g_pFBO->normalTexture_1_ID << std::endl;

		std::cout << "GL_MAX_COLOR_ATTACHMENTS = " << g_pFBO->getMaxColourAttachments() << std::endl;
		std::cout << "GL_MAX_DRAW_BUFFERS = " << g_pFBO->getMaxDrawBuffers() << std::endl;
	}


	g_pFBOGeometry = new cFBO();
	//geometry stencil buffer
	if (!g_pFBOGeometry->init(width, 1060, error))
	{
		std::cout << "FBO error: " << error << std::endl;
	}
	else
	{
		std::cout << "FBO is good." << std::endl;
		std::cout << "\tFBO ID = " << g_pFBO->ID << std::endl;
		std::cout << "\tcolour texture ID = " << g_pFBO->colourTexture_0_ID << std::endl;
		std::cout << "\tnormal texture ID = " << g_pFBO->normalTexture_1_ID << std::endl;
		std::cout << "GL_MAX_COLOR_ATTACHMENTS = " << g_pFBO->getMaxColourAttachments() << std::endl;
		std::cout << "GL_MAX_DRAW_BUFFERS = " << g_pFBO->getMaxDrawBuffers() << std::endl;
	}

	//set the initial values for the keys
	Keyboard::setKeys();

	//DO THE FBO THING FOR SHADOWS
	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	::g_pShaderManager->useShaderProgram("mySexyShader");
	glBindFramebuffer(GL_FRAMEBUFFER, g_pFBOGeometry->ID);
	float near_plane = 100.0f, far_plane = 500.f;
	g_lightProjection = glm::ortho(-75.f, 75.f, -75.f, 75.f, near_plane, far_plane);

	//get the uniform locations
	GLint bGetBrightness = glGetUniformLocation(sexyShaderID, "getBrightness");
	glUniform1i(bGetBrightness, GL_FALSE);
	GLint bIsSecondPassLocID = glGetUniformLocation(sexyShaderID, "bIsSecondPass");
	glUniform1i(bIsSecondPassLocID, GL_FALSE);
	GLint texShadowMapPosition = 13;
	GLint texShadowMap = glGetUniformLocation(sexyShaderID, "shadowMap");
	GLint uniLoc_idDepth = glGetUniformLocation(sexyShaderID, "bIsADepthBuffer");
	GLint bUseShadows = glGetUniformLocation(sexyShaderID, "useShadows");
	GLint bUseShadowMapAsTexture = glGetUniformLocation(sexyShaderID, "useShadowMapAsTexture");
	GLint vecLightPosLoc = glGetUniformLocation(sexyShaderID, "lightPos");
	GLint texFBOColour2DLocID = glGetUniformLocation(sexyShaderID, "texFBOColour2D");
	GLint texFBONormal2DLocID = glGetUniformLocation(sexyShaderID, "texFBONormal2D");
	GLint texFBOWorldPosition2DLocID = glGetUniformLocation(sexyShaderID, "texFBOVertexWorldPos2D");
	GLint texFBODepth2DLocID = glGetUniformLocation(sexyShaderID, "textFBODepth2D");

	//get the light offsets
	float zDistanceFromPlayerToLight = g_pCurrentScene->currentPlayer->thePlayerObject->position.z - g_mainLightPosition.z;
	float xDistanceFromPlayerToLight = g_pCurrentScene->currentPlayer->thePlayerObject->position.x - g_mainLightPosition.x;


	//create the gamepad
	g_pGamePad = new GamePad();

	// Main game or application loop
	while (!glfwWindowShouldClose(g_pGLFWWindow))
	{
		g_pGamePad->getIsConnected();
		g_pGamePad->getAxesArray();
		g_pGamePad->resolveJoystickInteractions();
		g_pGamePad->updateShoot();

		float ratio;
		int width, height;
		g_cameraTarget_XYZ = g_pCurrentScene->currentPlayer->thePlayerObject->position;
		g_cameraXYZ.x = g_pCurrentScene->currentPlayer->thePlayerObject->position.x;
		//float zDistanceFromPlayerToCamera = g_cameraXYZ.z - g_pCurrentScene->currentPlayer->thePlayerObject->position.z;

		//g_cameraXYZ.z = g_pCurrentScene->currentPlayer->thePlayerObject->position.z + zDistanceFromPlayerToCamera;
		//move the light based on the player location
		g_mainLightPosition.x = g_pCurrentScene->currentPlayer->thePlayerObject->position.x - xDistanceFromPlayerToLight;
		g_mainLightPosition.z = g_pCurrentScene->currentPlayer->thePlayerObject->position.z	- zDistanceFromPlayerToLight;

		glfwGetFramebufferSize(g_pGLFWWindow, &width, &height);
		ratio = SHADOW_WIDTH / (float)SHADOW_HEIGHT;
		//glViewport(0, 0, SHADOW_WIDTH, SHADOW_WIDTH);

		// Clear colour AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		::g_pShaderManager->useShaderProgram("mySexyShader");
		GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");

		// Update all the light uniforms...
		// (for the whole scene)
		::g_pLightManager->CopyLightInformationToCurrentShader();
	
		double curTime = glfwGetTime();
		double deltaTime = curTime - g_lastTimeStep;

		for (int i = 0; i < Keyboard::KeyCount; i++) {
			if (KeysPressed[i]) {
				Keyboard::key_resolution(i,deltaTime);
			}
		}

		g_pStateManager->determineStateBehaviours(g_pCurrentScene->entities);
		g_pStateManager->determineStateBehaviours(g_vecGameObjects);
		//check to see if projectiles are out of the range
		g_pCurrentScene->currentPlayer->checkProjectileBounds();

		//do this every frame so that the light can follow the player
		g_lightView = glm::lookAt(g_mainLightPosition, g_pCurrentScene->currentPlayer->thePlayerObject->position, glm::vec3(0.0, -1.0, 0.0));
		g_lightSpaceMatrix = g_lightProjection * g_lightView;
		//	 _____ __ __   ____  ___     ___   __    __      ____     ___  ____   ___      ___  ____       ____   ____  _____ _____
		//	/ ___/|  |  | /    ||   \   /   \ |  |__|  |    |    \   /  _]|    \ |   \    /  _]|    \     |    \ /    |/ ___// ___/
		// (   \_ |  |  ||  o  ||    \ |     ||  |  |  |    |  D  ) /  [_ |  _  ||    \  /  [_ |  D  )    |  o  )  o  (   \_(   \_ 
		//	\__  ||  _  ||     ||  D  ||  O  ||  |  |  |    |    / |    _]|  |  ||  D  ||    _]|    /     |   _/|     |\__  |\__  |
		//	/  \ ||  |  ||  _  ||     ||     ||  `  '  |    |    \ |   [_ |  |  ||     ||   [_ |    \     |  |  |  _  |/  \ |/  \ |
		//	\    ||  |  ||  |  ||     ||     | \      /     |  .  \|     ||  |  ||     ||     ||  .  \    |  |  |  |  |\    |\    |
		//	 \___||__|__||__|__||_____| \___/   \_/\_/      |__|\_||_____||__|__||_____||_____||__|\_|    |__|  |__|__| \___| \___|

		//::g_pShaderManager->useShaderProgram("mySexyShader");
		glBindFramebuffer(GL_FRAMEBUFFER, g_pFBOGeometry->ID);
		glUniform1i(bIsSecondPassLocID, GL_FALSE);

		//set the depth drawing to true
		g_pFBOGeometry->clearBuffers();
		//change the camera position
		glUniform1i(bUseShadows, GL_TRUE);
		glUniform1i(uniLoc_idDepth, GL_TRUE);
		//render all the objects to the shadow map buffer
		renderScene(g_pCurrentScene->terrain, ::g_pGLFWWindow,3, deltaTime);
		renderScene(g_pCurrentScene->entities, ::g_pGLFWWindow, 3, deltaTime);
		renderScene(g_pCurrentScene->powerUps, ::g_pGLFWWindow, 3, deltaTime);
		renderScene(g_pCurrentScene->currentPlayer->projectilesToDraw, ::g_pGLFWWindow, 3, deltaTime);
		renderScene(g_pCurrentScene->playerShield->pShieldBodies, ::g_pGLFWWindow, 3, deltaTime);
		renderScene(g_vecGameObjects, ::g_pGLFWWindow, 3, deltaTime);
		renderScene(g_pCurrentScene->obstacles, ::g_pGLFWWindow, 3, deltaTime);

		//update all active tree emitters
		for (int i = 0; i < g_pCurrentScene->terrain.size(); i++) {
			if (g_pCurrentScene->terrain[i]->theParticleEmitter->getActive() == true) {
				g_pCurrentScene->terrain[i]->theParticleEmitter->Update(deltaTime, glm::vec3(0.f));
				g_pCurrentScene->terrain[i]->theParticleEmitter->renderParticles(deltaTime, sexyShaderID, g_pCurrentScene->terrain[i]->position);
			}
		}

		//   ___                _             ___               
		//  | _ \ ___  _ _   __| | ___  _ _  | _ \ __ _  ___ ___
		//  |   // -_)| ' \ / _` |/ -_)| '_| |  _// _` |(_-<(_-<
		//  |_|_\\___||_||_|\__,_|\___||_|   |_|  \__,_|/__//__/
		//   
		//set the shadow map to go to the shadowMap uniform position

		////set the depth drawing to false
		glBindFramebuffer(GL_FRAMEBUFFER, g_pFBO->ID);
		g_pFBO->clearBuffers();
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUniform3f(vecLightPosLoc, g_mainLightPosition.x, g_mainLightPosition.y, g_mainLightPosition.z);
		//useShadows
		glUniform1i(bUseShadows, GL_FALSE);		
		glUniform1i(uniLoc_idDepth, GL_FALSE);
		//glUniform1f(bUseShadowMapAsTexture,GL_TRUE);

		GLint texFBOWorldPosition2DTextureUnitID = 37;
		glActiveTexture(GL_TEXTURE0 + texFBOWorldPosition2DTextureUnitID);
		glBindTexture(GL_TEXTURE_2D, g_pFBOGeometry->vertexWorldPos_2_ID);
		glUniform1i(texFBOWorldPosition2DLocID, texFBOWorldPosition2DTextureUnitID);

		GLint textFBODepth2DTextureID = 38;
		glActiveTexture(GL_TEXTURE0 + textFBODepth2DTextureID);
		glBindTexture(GL_TEXTURE_2D, g_pFBOGeometry->depthTexture_ID);
		glUniform1i(texFBODepth2DLocID, textFBODepth2DTextureID);

		//render everything
		//glUniform1i(bIsSecondPassLocID, GL_FALSE);
		renderScene(g_pCurrentScene->terrain, ::g_pGLFWWindow, 0,deltaTime);
		renderScene(g_pCurrentScene->entities, ::g_pGLFWWindow, 0, deltaTime);
		renderScene(g_pCurrentScene->powerUps, ::g_pGLFWWindow, 0, deltaTime);
		renderScene(g_pCurrentScene->currentPlayer->projectilesToDraw, ::g_pGLFWWindow, 0, deltaTime);
		renderScene(g_pCurrentScene->playerShield->pShieldBodies, ::g_pGLFWWindow, 0, deltaTime);
		renderScene(g_vecGameObjects, ::g_pGLFWWindow, 0, deltaTime);
		renderScene(g_pCurrentScene->obstacles, ::g_pGLFWWindow, 0, deltaTime);

		//draw the particles
		//g_pCurrentScene->currentPlayer->updateEmitterPosition();
		//g_pCurrentScene->currentPlayer->emitParticles(deltaTime, sexyShaderID);

		//update all active tree emitters
		for (int i = 0; i < g_pCurrentScene->terrain.size(); i++) {
			if (g_pCurrentScene->terrain[i]->theParticleEmitter->getActive() == true) {
				g_pCurrentScene->terrain[i]->theParticleEmitter->Update(deltaTime, glm::vec3(0.f));
				g_pCurrentScene->terrain[i]->theParticleEmitter->renderParticles(deltaTime, sexyShaderID, g_pCurrentScene->terrain[i]->position);
			}
		}

		//    ___         __                         _   ___                _             ___               
		//   |   \  ___  / _| ___  _ _  _ _  ___  __| | | _ \ ___  _ _   __| | ___  _ _  | _ \ __ _  ___ ___
		//   | |) |/ -_)|  _|/ -_)| '_|| '_|/ -_)/ _` | |   // -_)| ' \ / _` |/ -_)| '_| |  _// _` |(_-<(_-<
		//   |___/ \___||_|  \___||_|  |_|  \___|\__,_| |_|_\\___||_||_|\__,_|\___||_|   |_|  \__,_|/__//__/
		//   
		//FINAL PASS TO RENDER THE FULL IMAGE TO A SKYBOX
		//set the depth drawing to false
		//glUniform1i(uniLoc_idDepth, 0);
		
		std::vector<cGameObject*> skybox;
		skybox.push_back(g_pCurrentScene->terrain[0]);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//glUniform1i(bUseShadows, GL_FALSE);
		//glUniform1i(uniLoc_idDepth, GL_FALSE);
		////bind the shadow map
		//glActiveTexture(GL_TEXTURE0 + 25);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		//glUniform1i(texShadowMap, shadowUnit);


		GLint texFBOColour2DTextureUnitID = 45;
		glActiveTexture(GL_TEXTURE0 + texFBOColour2DTextureUnitID);
		glBindTexture(GL_TEXTURE_2D, g_pFBO->colourTexture_0_ID);
		glUniform1i(texFBOColour2DLocID, texFBOColour2DTextureUnitID);

		GLint texFBONormal2DTextureUnitID = 46;
		glActiveTexture(GL_TEXTURE0 + texFBONormal2DTextureUnitID);
		glBindTexture(GL_TEXTURE_2D, g_pFBO->normalTexture_1_ID);
		glUniform1i(texFBONormal2DLocID, texFBONormal2DTextureUnitID);

		GLint texFBOWorldPosition2DTextureUnitID2 = 47;
		glActiveTexture(GL_TEXTURE0 + texFBOWorldPosition2DTextureUnitID2);
		glBindTexture(GL_TEXTURE_2D, g_pFBO->vertexWorldPos_2_ID);
		glUniform1i(texFBOWorldPosition2DLocID, texFBOWorldPosition2DTextureUnitID2);

		glUniform1i(bIsSecondPassLocID, GL_TRUE);
		glUniform1i(bGetBrightness, GL_TRUE);

		// Set the sampler in the shader to the same texture unit (20)

		GLint screenWidthLocID = glGetUniformLocation(sexyShaderID, "screenWidth");
		GLint screenHeightLocID = glGetUniformLocation(sexyShaderID, "screenHeight");
		glUniform1f(screenWidthLocID, (float)width);
		glUniform1f(screenHeightLocID, (float)height);

		renderScene(skybox, ::g_pGLFWWindow,0,deltaTime);



		// Projection and view don't change per scene (maybe)
		glm::mat4x4 p = glm::perspective(0.6f,			// FOV
			ratio,		// Aspect ratio
			0.1f,			// Near (as big as possible)
			1000.0f);	// Far (as small as possible)

						// View or "camera" matrix
		glm::mat4 v = glm::mat4(1.0f);	// identity

		v = glm::lookAt(g_cameraXYZ,		// "eye" or "camera" position
			g_cameraTarget_XYZ,				// "At" or "target" 
			glm::vec3(0.0f, 1.0f, 0.0f));	// "up" vector

		::g_pTheDebugrender->RenderDebugObjects(v, p, deltaTime);

		//shakes the camera
		shakeCamera(deltaTime);

		g_pCurrentScene->currentPlayer->applyContinuousDirectionalVelocityToProjectiles();
		//step the physics
  		theWorld->TimeStep(curTime,deltaTime);
		synchronizeGraphicsWithPhysicsLibrary();

		//switch back
		g_lastTimeStep = curTime;
		glfwSwapBuffers(g_pGLFWWindow);
		glfwPollEvents();
	}// while ( ! glfwWindowShouldClose(window) )

	glfwDestroyWindow(g_pGLFWWindow);
	glfwTerminate();

	delete ::g_pTheDebugrender;
	delete ::g_pTextureManager;
	delete ::g_pShaderManager;
	delete ::g_pVAOManager;
	delete ::g_pLightManager;
	delete ::g_GameTerrain;
	delete ::g_curGameObject;
	delete ::g_GameTerrain;
	delete ::g_pSceneManager;
	delete ::g_pCurrentScene;
	delete ::g_pParticleManager;
	delete ::g_pSoundManager;
	delete ::g_pCollisionHandler;
	delete ::g_pStateManager;
	return 0;
}

void printError(const std::string& error) {
	std::cout << error << std::endl;
}

void shakeCamera(float deltaTime) {
	

	if (g_shakeCamera) {
		currentShakeTime += deltaTime;
		if (currentShakeTime < TimeToShake) {
			g_cameraXYZ.x = getRandInRange(g_cameraXYZ.x - 2.f, g_cameraXYZ.x + 2.f);
			g_cameraXYZ.y = getRandInRange(g_cameraXYZ.y - 2.f, g_cameraXYZ.y + 2.f);
			//g_cameraXYZ.z = getRandInRange(g_cameraXYZ.z - 2.f, g_cameraXYZ.z + 2.f);
		}
		else {
			g_shakeCamera = false;
			g_cameraXYZ = g_cameraBackupPosition;
			currentShakeTime = 0.f;
		}
	}

}
