#ifndef _cVAOMeshManager_HG_
#define _cVAOMeshManager_HG_

//#include <GLFW\glfw3.h>	// Give us opengl.h 
#include <string>
//#include "cMesh.h"
// Instead of including, which means possibly including 
//	stuff we don't need or want to know about, we 
//  'forward declare'
class cMesh;		// Forward declare

#include <map>		// "dictonary" or PHP array
#include "sVAOInfo.h"

// This will handle: 
// - taking a cMesh and loading into the various buffers
// - take a mesh "name" and returning the desired VAO info 
//   (for rendering)
class cVAOMeshManager
{
public:
	cVAOMeshManager();
	~cVAOMeshManager();
	// Take name from mesh for lookup (for rendering)
	bool loadMeshIntoVAO(cMesh &theMesh, int shaderID, bool bKeepMesh = false);
	// During rendering (aka 'drawing'), get the info we need.
	bool lookupVAOFromName(std::string name, sVAOInfo &theVAOInfo);
	// Return mesh by name
	bool lookupMeshFromName(std::string name, cMesh &theMesh);
	std::map<int, std::string> mapNumberToName;
private:
	// 1st is what I'm indexing by (i.e. type)
	// 2nd is what I'm actually storing (the type)
	std::map< std::string, sVAOInfo > m_mapNameToVAO;
	// The dynamic VAOs
	// Meshes that I want to keep around (like the terrain?)
	std::map< std::string, cMesh > m_mapNameToMesh;
};
#endif