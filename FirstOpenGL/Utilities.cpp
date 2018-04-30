#include "Utilities.h"
#include "cGameObject.h"
#include "cTriangle.h"
#include "CTexUnitInfoBrief.h"
#include <iPhysicsFactory.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>	// RAND_MAX
#include <map>	
#include <windows.h>
#include "cSceneManager.h"
#include "cPlayer.h"
#include "cShieldDescription.h"

extern nPhysics::iPhysicsFactory* theFactory;
typedef nPhysics::iPhysicsFactory* (*f_CreateFactory)();
extern sScene* g_pCurrentScene;

bool loadConfigFile(int& width,int& height,std::string& title) {
	std::ifstream infoFile("config.txt");
	if (!infoFile.is_open())
	{	// File didn't open...
		std::cout << "Can't find config file" << std::endl;
		std::cout << "Using defaults" << std::endl;
		return false;
	}
	else
	{	// File DID open, so read it... 
		std::string a;

		infoFile >> a;	// "Game"	//std::cin >> a;
		infoFile >> a;	// "Config"
		infoFile >> a;	// "width"

		infoFile >> width;	// 1080

		infoFile >> a;	// "height"

		infoFile >> height;	// 768

		infoFile >> a;		// Title_Start

		std::stringstream ssTitle;		// Inside "sstream"
		bool bKeepReading = true;
		do
		{
			infoFile >> a;		// Title_End??
			if (a != "Title_End")
			{
				ssTitle << a << " ";
			}
			else
			{	// it IS the end! 
				bKeepReading = false;
				title = ssTitle.str();
			}
		} while (bKeepReading);


	}//if ( ! infoFile.is_open() )

	return true;
}

glm::vec3 getTriangleCentroid(cPhysTriangle* theTriangle) {
	glm::vec3 theCenter(0.0f);

	theCenter.x = (theTriangle->vertex[0].x + theTriangle->vertex[0].x + theTriangle->vertex[0].x) / 3;
	theCenter.y = (theTriangle->vertex[1].y + theTriangle->vertex[1].y + theTriangle->vertex[1].y) / 3;
	theCenter.z = (theTriangle->vertex[2].z + theTriangle->vertex[2].z + theTriangle->vertex[2].z) / 3;

	return theCenter;
}

glm::vec3 getTriangleCentroid(glm::vec3 theTriangle[3]) {
	glm::vec3 theCenter(0.0f);

	theCenter.x = (theTriangle[0].x + theTriangle[0].x + theTriangle[0].x) / 3;
	theCenter.y = (theTriangle[1].y + theTriangle[1].y + theTriangle[1].y) / 3;
	theCenter.z = (theTriangle[2].z + theTriangle[2].z + theTriangle[2].z) / 3;

	return theCenter;
}

glm::vec3 getTriangleCentroid(glm::vec3* theTriangle[3]) {
	glm::vec3 theCenter(0.0f);

	theCenter.x = (theTriangle[0]->x + theTriangle[0]->x + theTriangle[0]->x) / 3;
	theCenter.y = (theTriangle[1]->y + theTriangle[1]->y + theTriangle[1]->y) / 3;
	theCenter.z = (theTriangle[2]->z + theTriangle[2]->z + theTriangle[2]->z) / 3;

	return theCenter;
}

void getXYFromRadiusAndAngle(float& x, float&y, float radius, float angle)
{
	x = radius * cos(angle);
	y = radius * sin(angle);
}

namespace QnDTexureSamplerUtility
{
	bool bIsLoaded = false;

	// 2D textures:
	GLint texSamp2D00_LocID = -1;
	GLint texSamp2D01_LocID = -1;
	GLint texSamp2D02_LocID = -1;
	GLint texSamp2D03_LocID = -1;
	GLint texSamp2D04_LocID = -1;
	GLint texSamp2D05_LocID = -1;
	GLint texSamp2D06_LocID = -1;
	GLint texSamp2D07_LocID = -1;

	GLint texBlend00_LocID = -1;
	GLint texBlend01_LocID = -1;
	GLint texBlend02_LocID = -1;
	GLint texBlend03_LocID = -1;
	GLint texBlend04_LocID = -1;
	GLint texBlend05_LocID = -1;
	GLint texBlend06_LocID = -1;
	GLint texBlend07_LocID = -1;

	// Cube map textures
	GLint texSampCube00_LocID = -1;
	GLint texSampCube01_LocID = -1;
	GLint texSampCube02_LocID = -1;
	GLint texSampCube03_LocID = -1;

	GLint texCubeBlend00_LocID = -1;
	GLint texCubeBlend01_LocID = -1;
	GLint texCubeBlend02_LocID = -1;
	GLint texCubeBlend03_LocID = -1;

	void LoadUniformLocationsIfNeeded(unsigned int shaderID)
	{
		if (!bIsLoaded)
		{	// Load the uniform locations
			texSamp2D00_LocID = glGetUniformLocation(shaderID, "texSamp2D00");
			texSamp2D01_LocID = glGetUniformLocation(shaderID, "texSamp2D01");
			texSamp2D02_LocID = glGetUniformLocation(shaderID, "texSamp2D02");
			//texSamp2D03_LocID = glGetUniformLocation(shaderID, "texSamp2D03");
			//texSamp2D04_LocID = glGetUniformLocation(shaderID, "texSamp2D04");
			//texSamp2D05_LocID = glGetUniformLocation(shaderID, "texSamp2D05");
			//texSamp2D06_LocID = glGetUniformLocation(shaderID, "texSamp2D06");
			//texSamp2D07_LocID = glGetUniformLocation(shaderID, "texSamp2D07");

			texBlend00_LocID = glGetUniformLocation(shaderID, "texBlend00");
			texBlend01_LocID = glGetUniformLocation(shaderID, "texBlend01");
			texBlend02_LocID = glGetUniformLocation(shaderID, "texBlend02");
			//texBlend03_LocID = glGetUniformLocation(shaderID, "texBlend03");
			//texBlend04_LocID = glGetUniformLocation(shaderID, "texBlend04");
			//texBlend05_LocID = glGetUniformLocation(shaderID, "texBlend05");
			//texBlend06_LocID = glGetUniformLocation(shaderID, "texBlend06");
			//texBlend07_LocID = glGetUniformLocation(shaderID, "texBlend07");

			texSampCube00_LocID = glGetUniformLocation(shaderID, "texSampCube00");
			texSampCube01_LocID = glGetUniformLocation(shaderID, "texSampCube00");
			//texSampCube02_LocID = glGetUniformLocation(shaderID, "texSampCube00");
			//texSampCube03_LocID = glGetUniformLocation(shaderID, "texSampCube00");

			texCubeBlend00_LocID = glGetUniformLocation(shaderID, "texCubeBlend00");
			texCubeBlend01_LocID = glGetUniformLocation(shaderID, "texCubeBlend01");
			//texCubeBlend02_LocID = glGetUniformLocation(shaderID, "texCubeBlend02");
			//texCubeBlend03_LocID = glGetUniformLocation(shaderID, "texCubeBlend03");

			bIsLoaded = true;	// So we won't call these again
		}

		return;
	}// void LoadUniformLocationsIfNeeded()


	void setAllSamplerUnitsToInvalidNumber(GLint invalidTextureUnit)
	{
		glUniform1i(texSamp2D00_LocID, invalidTextureUnit);
		glUniform1i(texSamp2D01_LocID, invalidTextureUnit);
		glUniform1i(texSamp2D02_LocID, invalidTextureUnit);
		//glUniform1i(texSamp2D03_LocID, invalidTextureUnit);
		//glUniform1i(texSamp2D04_LocID, invalidTextureUnit);
		//glUniform1i(texSamp2D05_LocID, invalidTextureUnit);
		//glUniform1i(texSamp2D06_LocID, invalidTextureUnit);
		//glUniform1i(texSamp2D07_LocID, invalidTextureUnit);

		// Cube map textures
		glUniform1i(texSampCube00_LocID, invalidTextureUnit);
		glUniform1i(texSampCube01_LocID, invalidTextureUnit);
		//glUniform1i(texSampCube02_LocID, invalidTextureUnit);
		//glUniform1i(texSampCube03_LocID, invalidTextureUnit);

		return;
	}//void setAllSamplerUnitsToInvalidNumber()

	void clearAllBlendValuesToZero(void)
	{
		glUniform1f(texBlend00_LocID, 0.0f);
		glUniform1f(texBlend01_LocID, 0.0f);
		glUniform1f(texBlend02_LocID, 0.0f);
		//glUniform1f(texBlend03_LocID, 0.0f);
		//glUniform1f(texBlend04_LocID, 0.0f);
		//glUniform1f(texBlend05_LocID, 0.0f);
		//glUniform1f(texBlend06_LocID, 0.0f);
		//glUniform1f(texBlend07_LocID, 0.0f);

		glUniform1f(texCubeBlend00_LocID, 0.0f);
		glUniform1f(texCubeBlend01_LocID, 0.0f);
		glUniform1f(texCubeBlend02_LocID, 0.0f);
		//glUniform1f(texCubeBlend03_LocID, 0.0f);

		return;
	}//void clearAllBlendValuesToZero()

	 // Handy function to set sampelrs
	void set2DSamplerAndBlenderByIndex(GLint samplerIndex, float blendRatio, GLint textureUnitID)
	{
		switch (samplerIndex)
		{
		case 0:
			glUniform1i(texSamp2D00_LocID, textureUnitID);
			glUniform1f(texBlend00_LocID, blendRatio);
			break;
		case 1:
			glUniform1i(texSamp2D01_LocID, textureUnitID);
			glUniform1f(texBlend01_LocID, blendRatio);
			break;
		case 2:
			glUniform1i(texSamp2D02_LocID, textureUnitID);
			glUniform1f(texBlend02_LocID, blendRatio);
			break;
		//case 3:
		//	glUniform1i(texSamp2D03_LocID, textureUnitID);
		//	glUniform1f(texBlend03_LocID, blendRatio);
		//	break;
		//case 4:
		//	glUniform1i(texSamp2D04_LocID, textureUnitID);
		//	glUniform1f(texBlend04_LocID, blendRatio);
		//	break;
		//case 5:
		//	glUniform1i(texSamp2D05_LocID, textureUnitID);
		//	glUniform1f(texBlend05_LocID, blendRatio);
		//	break;
		//case 6:
		//	glUniform1i(texSamp2D06_LocID, textureUnitID);
		//	glUniform1f(texBlend06_LocID, blendRatio);
		//	break;
		//case 7:
		//	glUniform1i(texSamp2D07_LocID, textureUnitID);
		//	glUniform1f(texBlend07_LocID, blendRatio);
		//	break;
		default:
			// Invalid samplerIndex
			break;
		}//switch (samplerIndex)
		return;
	}//void set2DSamplerAndBlenderByIndex()

	void setCubeSamplerAndBlenderByIndex(GLint samplerIndex, float blendRatio, GLint textureUnitID)
	{
		switch (samplerIndex)
		{
		case 0:
			glUniform1i(texSampCube00_LocID, textureUnitID);
			glUniform1f(texCubeBlend00_LocID, blendRatio);
			break;
		case 1:
			glUniform1i(texSampCube01_LocID, textureUnitID);
			glUniform1f(texCubeBlend01_LocID, blendRatio);
			break;
		//case 2:
		//	glUniform1i(texSampCube02_LocID, textureUnitID);
		//	glUniform1f(texCubeBlend02_LocID, blendRatio);
		//	break;
		//case 3:
		//	glUniform1i(texSampCube03_LocID, textureUnitID);
		//	glUniform1f(texCubeBlend03_LocID, blendRatio);
		//	break;
		default:
			// Invalid samplerIndex;
			break;
		}//switch (samplerIndex)
		return;
	}//void setCubeSamplerAndBlenderByIndex()
	
	
	void SetSamplersForMeshTextures(cGameObject &object,
		std::map<std::string /*textureName*/, CTexUnitInfoBrief> &mapTexAndUnitInfo)
	{
		// 2D textures first
		int numTextures = (int)object.vecMehs2DTextures.size();
		for (int samplerIndex = 0; samplerIndex != numTextures; samplerIndex++)
		{
			// What texture unit is this texture set to?
			std::map<std::string, CTexUnitInfoBrief>::iterator itTextUnitInfo
				= mapTexAndUnitInfo.find(object.vecMehs2DTextures[samplerIndex].name);
			// Have we mapped that one?
			if (itTextUnitInfo != mapTexAndUnitInfo.end())
			{	// Yes, so assign it
				set2DSamplerAndBlenderByIndex(samplerIndex,
					object.vecMehs2DTextures[samplerIndex].blendRatio,
					itTextUnitInfo->second.texUnitNumOffsetBy_GL_TEXTURE0);
				// Set blend function, too
			}
		}// 2D textures

		 // Now cube maps
		numTextures = (int)object.vecMeshCubeMaps.size();
		for (int samplerIndex = 0; samplerIndex != numTextures; samplerIndex++)
		{
			// What texture unit is this texture set to?
			std::map<std::string, CTexUnitInfoBrief>::iterator itTextUnitInfo
				= mapTexAndUnitInfo.find(object.vecMeshCubeMaps[samplerIndex].name);
			// Have we mapped that one?
			if (itTextUnitInfo != mapTexAndUnitInfo.end())
			{	// Yes, so assign it
				setCubeSamplerAndBlenderByIndex(samplerIndex,
					object.vecMeshCubeMaps[samplerIndex].blendRatio,
					itTextUnitInfo->second.texUnitNumOffsetBy_GL_TEXTURE0);
			}
		}// cube maps
	}//void SetSamplersForMeshTextures()

	void SetSamplersForMeshTextures(sMeshDrawInfo &meshDrawInfo,
		std::map<std::string /*textureName*/, CTexUnitInfoBrief> &mapTexAndUnitInfo)
	{
		// 2D textures first
		int numTextures = (int)meshDrawInfo.vecMehs2DTextures.size();
		for (int samplerIndex = 0; samplerIndex != numTextures; samplerIndex++)
		{
			// What texture unit is this texture set to?
			std::map<std::string, CTexUnitInfoBrief>::iterator itTextUnitInfo
				= mapTexAndUnitInfo.find(meshDrawInfo.vecMehs2DTextures[samplerIndex].name);
			// Have we mapped that one?
			if (itTextUnitInfo != mapTexAndUnitInfo.end())
			{	// Yes, so assign it
				set2DSamplerAndBlenderByIndex(samplerIndex,
					meshDrawInfo.vecMehs2DTextures[samplerIndex].blendRatio,
					itTextUnitInfo->second.texUnitNumOffsetBy_GL_TEXTURE0);
				// Set blend function, too
			}
		}// 2D textures

		 // Now cube maps
		numTextures = (int)meshDrawInfo.vecMeshCubeMaps.size();
		for (int samplerIndex = 0; samplerIndex != numTextures; samplerIndex++)
		{
			// What texture unit is this texture set to?
			std::map<std::string, CTexUnitInfoBrief>::iterator itTextUnitInfo
				= mapTexAndUnitInfo.find(meshDrawInfo.vecMeshCubeMaps[samplerIndex].name);
			// Have we mapped that one?
			if (itTextUnitInfo != mapTexAndUnitInfo.end())
			{	// Yes, so assign it
				setCubeSamplerAndBlenderByIndex(samplerIndex,
					meshDrawInfo.vecMeshCubeMaps[samplerIndex].blendRatio,
					itTextUnitInfo->second.texUnitNumOffsetBy_GL_TEXTURE0);
			}
		}// cube maps
	}

};//namespace QnDTexureSamplerUtility

void InitPhysics() {	

	//if debug use the debug dll else use release dll
#if _DEBUG
	std::string libraryFile = "MyBulletLibrary.dll";
#else
	std::string libraryFile = "MyBulletLibrary_release.dll";
#endif

	std::string createFactoryName = "CreateFactory";

	HINSTANCE hGetProckDll = LoadLibraryA(libraryFile.c_str());
	if (!hGetProckDll)
	{
		std::cout << "oh no!" << std::endl;
	}

	f_CreateFactory CreateFactory = 0;

	CreateFactory = (f_CreateFactory)GetProcAddress(hGetProckDll, createFactoryName.c_str());
	if (!CreateFactory)
	{
		std::cout << "where's the CreateFactory???" << std::endl;
		system("pause");

	}

	theFactory = CreateFactory();
}

void ReadFileToToken(std::ifstream &file, std::string token)
{
	bool bKeepReading = true;
	std::string garbage;
	do
	{
		file >> garbage;		// Title_End??
		if (garbage == token)
		{
			return;
		}
	} while (bKeepReading);
	return;
}

void synchronizeGraphicsWithPhysicsLibrary() {
	//update the entities 
	for (int i = 1; i < g_pCurrentScene->entities.size(); i++) {
		g_pCurrentScene->entities[i]->synchronize();
	}

	//update the items
	for (int i = 0; i < g_pCurrentScene->powerUps.size(); i++) {
		g_pCurrentScene->powerUps[i]->synchronize();
	}

	//update the obstacles
	for (int i = 0; i < g_pCurrentScene->obstacles.size(); i++) {
		g_pCurrentScene->obstacles[i]->synchronize();
	}

	//update the shield
	for (int i = 0; i < g_pCurrentScene->playerShield->pShieldBodies.size(); i++) {
		g_pCurrentScene->playerShield->pShieldBodies[i]->synchronize();
	}

	//g_pCurrentScene->currentPlayer->synchronize();

	//update player projectiles
	for (int i = 0; i < g_pCurrentScene->currentPlayer->projectilesToDraw.size(); i++) {
		nPhysics::iRigidBody* tempBody = dynamic_cast<nPhysics::iRigidBody*>(g_pCurrentScene->currentPlayer->projectilesToDraw[i]->theBody);
		glm::vec3 tempPos;
		glm::vec3 tempRot;
		tempBody->GetPosition(tempPos);
		tempBody->GetRotation(tempRot);
		g_pCurrentScene->currentPlayer->projectilesToDraw[i]->position = tempPos;
		g_pCurrentScene->currentPlayer->projectilesToDraw[i]->orientation2 = tempPos;
	}
}
