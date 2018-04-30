#ifndef _cMesh_HG_
#define _cMesh_HG_

//#include "sVertex_xyz_rgba_n_uv2_bt.h"
#include "sVertex_xyz_rgba_n_uv2_bt_4Bones.h"

#include "cTriangle.h" 

static const int MAX_VERTEX_ARRAY_SIZE = 4096;
static const int MAX_INDEX_ARRAY_SIZE = 4096;

#include <string>
#include <glm/vec3.hpp>
#include <vector>

class cMesh
{
public:
	cMesh();
	~cMesh();

	std::string name;	// the friendly name of the mesh

	// Array of vertices 
//	cVertex_xyz_rgb vertices[MAX_VERTEX_ARRAY_SIZE]; // Stack
	sVertex_xyz_rgba_n_uv2_bt_4Bones* pVertices;						// Heap
	int numberOfVertices;

	// Array of triangles (indices)
//	cTriangle triangles[MAX_INDEX_ARRAY_SIZE];	// Stack
	cTriangle* pTriangles;						// Heap
	int numberOfTriangles;

	// Used for the physics calculations
	std::vector< cPhysTriangle > vecPhysTris;
	void GeneratePhysicsTriangles(void);
	// Takes an indexed model and makes just a vertex array model
	// WARNING: Will OVERWRITE the vertex array 
	void FlattenIndexedModel(void);

	// First 2 are the "bounding box" edges
	glm::vec3 minXYZ;
	glm::vec3 maxXYZ;
	// This is Max - Min for each x,y,z
	glm::vec3 maxExtentXYZ;	

	//for AABB
	glm::vec3 maxZ;
	glm::vec3 minZ;
	glm::vec3 maxY;
	glm::vec3 minY;
	glm::vec3 maxX;
	glm::vec3 minX;

	float maxExtent;		// Biggest of above
	// Scale can be 1.0 div maxExtent;
	float scaleForUnitBBox;
	void CalculateExtents(void);
	//void CalculateNormals(void);
};


#endif
