#ifndef  _RenderUtilities_HG_
#define _RenderUtilities_HG_
#include "globalOpenGL_GLFW.h"
#include "cGameObject.h"


bool renderScene(std::vector<cGameObject*> theObjects, GLFWwindow* theWindow,int type, float deltaTime);

void DrawObject(cGameObject* pTheGO, GLint curShaderProgramID, float deltaTime, bool instanced, int numInstancedParticles);

void CalculateSkinnedMeshBonesAndLoad(sMeshDrawInfo &theMesh, cGameObject* pTheGO,
	unsigned int UniformLoc_numBonesUsed,
	unsigned int UniformLoc_bonesArray);
#endif // ! _RenderUtilities_HG_
