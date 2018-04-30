#ifndef _ModelUtilities_HG_
#define _ModelUtilities_HG_

#include <fstream>
#include <string>
#include "cMesh.h"

class cVAOMeshManager;
class cLight;


// Note the & symbol which is "by reference" which means
//	it's NOT a copy, but referring to the original object.
// ALSO note that this is a C++ thing (& isn't a thing in C)
bool LoadTexturesFromFile(std::string& fileName);
bool MapTexturesToProperObjects();
bool Load3DModelsFromModelFileWithNormalsAndUV(int shaderID, cVAOMeshManager* pVAOManager, std::string &error);
bool Load3DModelsFromModelFileWithNormals(int shaderID, cVAOMeshManager* pVAOManager, std::string &error);

bool Load3DModelsFromModelFile(int shaderID, cVAOMeshManager* pVAOManager, std::string &error);
bool Load3DModelsIntoMeshManager(int shaderID, cVAOMeshManager* pVAOManager, std::string &error);
bool LoadAnimations(int shaderId);
bool LoadLightsAttributesFromFile(std::string& fileName, std::vector<cLight>& theLights);

#endif
