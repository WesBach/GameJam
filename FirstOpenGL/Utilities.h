#ifndef _Utilities_HG_
#define _Utilities_HG_
#include <string>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glad\glad.h>
#include <fstream>
#include <sstream>
#include <map>


class cPhysTriangle;
class sMeshDrawInfo;
class cGameObject;
class CTexUnitInfoBrief;
class iPhysicsWorld;

// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation
template <class T>
T getRandInRange( T min, T max )
{
	double value = 
		min + static_cast <double> (rand()) 
		/ ( static_cast <double> (RAND_MAX/(static_cast<double>(max-min) )));
	return static_cast<T>(value);
}

bool loadConfigFile(int& width, int& height, std::string& title);

glm::vec3 getTriangleCentroid(glm::vec3 theTriangles[3] );
glm::vec3 getTriangleCentroid(cPhysTriangle* theTriangle);
glm::vec3 getTriangleCentroid(glm::vec3* theTriangle[3]);

void getXYFromRadiusAndAngle(float& x, float&y, float radius, float angle);
// Only "function signature"
void ReadFileToToken(std::ifstream &file, std::string token);

namespace QnDTexureSamplerUtility
{
	void LoadUniformLocationsIfNeeded(unsigned int shaderID);
	void setAllSamplerUnitsToInvalidNumber(GLint invalidTextureUnit);
	void clearAllBlendValuesToZero(void);
	void SetSamplersForMeshTextures(sMeshDrawInfo &meshDrawInfo,std::map<std::string /*textureName*/, CTexUnitInfoBrief> &mapTexAndUnitInfo);
	//	void set2DSamplerAndBlenderByIndex(GLint samplerIndex, float blendRatio, GLint textureUnitID );
	//	void setCubeSamplerAndBlenderByIndex( GLint samplerIndex, float blendRatio, GLint textureUnitID );
	void SetSamplersForMeshTextures(cGameObject &object,std::map<std::string /*textureName*/, CTexUnitInfoBrief> &mapTexAndUnitInfo);
};

void InitPhysics();
void synchronizeGraphicsWithPhysicsLibrary();
#endif
