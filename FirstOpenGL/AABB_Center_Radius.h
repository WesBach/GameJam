#ifndef _AABB_Center_Radius_HG_
#define _AABB_Center_Radius_HG_
#include <glm\vec3.hpp>
#include <vector>

class AABB_Tri;
class cMesh;
class cGameObject;

class AABB_Center_Radius
{	
public:
	AABB_Center_Radius();
	~AABB_Center_Radius();
	//pass in the mesh and get the extents to build the box verts
	AABB_Center_Radius(cMesh* theMesh, glm::vec3& theObjectPos, float& scale);
	glm::vec3 center; // center point of AABB
	glm::vec3 radius; // radius or halfwidth extents (rx, ry, rz) (distance from center to max of each point)
	//vertices to create from the object mesh and position
	std::vector<glm::vec3> pVertices;
	int numberOfVertices;
	float scale;
	std::string meshname;

	//for inner box update
	glm::vec3 positionOffest;

	unsigned long long calculateID(unsigned long meshID, unsigned long triID);
	void buildBoundingBox(float& scale);

	void adaptVerticesToNewCenter(glm::vec3& center);

	void AABB_Center_Radius::setMeshInfo(cMesh * theMesh, glm::vec3& pos, float& scale);
	void drawCollisionBox(glm::vec3 point, float size, glm::vec3 color);
	AABB_Tri* theTriangles[12];
	int numTriangles;
	std::vector<AABB_Center_Radius*> theInnerBoxes;

	//void generate inner boxes recursively given an integer of how many iterations to complete
	void generateAABBInSpecificLocation(glm::vec3 point,float size);
	void generateAABBinSpecificLocations(std::vector<glm::vec3> locs, std::vector<float> sizes);
	void generateAABBAtMeshExtents();
	unsigned int AABBDebugId;

	void update(cMesh* theMesh, glm::vec3& theObjectPos, float& scale);

	float getAverageRadius();
};
#endif // !_AABB_HG_