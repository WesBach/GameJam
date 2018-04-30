#ifndef _cSceneManager_HG_
#define _cSceneManager_HG_

#include <map>
#include <vector>
#include <string>
#include <sRigidBodyDesc.h>
#include <iShape.h>
//#include "ModelUtilities.h"

class cGameObject;
class cVAOMeshManager;
class cMesh;
class cEnemy;
class cPlayer;
class cPowerUp;
class iEntity;
class cShieldDescription;

struct sScene {
	sScene(); 
	~sScene();
	//for ease of drawing
	std::vector<cGameObject*>	entities;
	std::vector<cGameObject*>	terrain;
	std::vector<cGameObject*>	powerUps;
	std::vector<cGameObject*>   obstacles;

	//for scene interactions
	cPlayer*					currentPlayer;
	cGameObject*				currentBoss;
	cShieldDescription*			playerShield;
	std::vector<iEntity*>		sceneEntities;
	std::vector<cPowerUp*>		scenePowerUps;
	void setPlayerPointer();
};

class cSceneManager {
public :
	cSceneManager();
	bool loadSceneFromFileIntoSceneMap(std::string& fileName,int mapIndex, cVAOMeshManager* theMeshManager);
	bool loadModelsFromModelInfoFile(std::string& filename, cVAOMeshManager* theMeshManager,int shaderId);

	sScene getSceneById(int id);//copy the vector, we dont want to change the original
	void copySceneFromCopyToPointer(const sScene& copyFrom, sScene* copyTo);
	void populateEnemies(std::vector<cEnemy>& enemies,sScene* theScene);
	void loadLevelTextures(sScene* theScene);
	void loadNextLevel(sScene* g_pCurrentScene, cPlayer* thePlayer);
	void configurePowerUpsForScene(sScene* theScene, std::vector<cPowerUp*>& thePowerUps);
	void loadConstraintsForScene(sScene* theScene);
	int getCurrentLevel();
	void setCurrentLevel(int levelIndex);
	void loadAssimpLoadedModelsIntoScene(sScene* theScene);
private:
	int currentLevel;
	int numLevels;

	void loadObjectData(std::ifstream& theFile, std::vector<cGameObject*>& theVector, cVAOMeshManager* theMeshManager);
	bool loadPlyFileIntoMeshWith_Normals_and_UV(std::string filename, cMesh &theMesh);
	bool loadPlyFileIntoMeshWithNormals(std::string filename, cMesh &theMesh);
	void createSceneRigidBodies(sScene* theScene, cVAOMeshManager* theMeshManager);
	std::map<int, sScene*> mSceneMap;
	void populateRigidBodyDescAndShape(const cGameObject& object,  nPhysics::sRigidBodyDesc& desc,nPhysics::iShape*& shape,cVAOMeshManager* theMeshManager);
};

#endif // !_cSceneManager_HG_

