#include "AABB_Center_Radius.h"
#include <glm\glm.hpp>
#include "cMesh.h"
#include "AABB_Tri.h"
#include "DebugRenderer.h"
#include "cVAOMeshManager.h"

extern DebugRenderer*	g_pTheDebugrender;
extern cVAOMeshManager*	g_pVAOManager;

AABB_Center_Radius::AABB_Center_Radius()
{
	this->center = glm::vec3(0.0f);
	this->radius = glm::vec3(0.0f);
	this->AABBDebugId = 0;
}

AABB_Center_Radius::~AABB_Center_Radius()
{

}


AABB_Center_Radius::AABB_Center_Radius(cMesh * theMesh, glm::vec3& pos,float& scale)
{
	//get the position for the box to be drawn
	this->meshname = theMesh->name;
	this->scale = scale;
	this->center = pos;
	this->radius.x = theMesh->maxExtentXYZ.x / 2;
	this->radius.y = theMesh->maxExtentXYZ.y / 2;
	this->radius.z = theMesh->maxExtentXYZ.z / 2;
}

void AABB_Center_Radius::update(cMesh* theMesh, glm::vec3& pos, float& scale) {
	//get the position for the box to be drawn
	this->meshname = theMesh->name;
	this->scale = scale;
	this->center = pos;
	this->radius.x = theMesh->maxExtentXYZ.x / 2;
	this->radius.y = theMesh->maxExtentXYZ.y / 2;
	this->radius.z = theMesh->maxExtentXYZ.z / 2;
}


void AABB_Center_Radius::setMeshInfo(cMesh * theMesh, glm::vec3& pos, float& scale)
{
	//get the position for the box to be drawn
	this->center = glm::vec3(0.0f);
	this->radius.x = (theMesh->minXYZ.x - theMesh->maxXYZ.x) / 2;//(distance from min to max point divided by 2)
	this->radius.y = (theMesh->minXYZ.y - theMesh->maxXYZ.y) / 2;
	this->radius.z = (theMesh->minXYZ.z - theMesh->maxXYZ.z) / 2;
	this->scale = scale;
}

void AABB_Center_Radius::buildBoundingBox(float& scale)
{
	//using the center and radius of the box create the bounding box outside of it
	glm::vec3 point0;
	this->pVertices.clear();

	//FRONT POINTS
	//front top right
	point0.x = this->center.x + (radius.x* scale); 
	point0.y = this->center.y + (radius.y* scale);
	point0.z = this->center.z + (radius.z* scale);
	this->pVertices.push_back(point0);

	//front bottom right
	glm::vec3 point1;
	point1.x = this->center.x + (radius.x* scale);
	point1.y = this->center.y - (radius.y* scale);
	point1.z = this->center.z + (radius.z* scale);
	this->pVertices.push_back(point1);

	//front top left 
	glm::vec3 point2;
	point2.x = this->center.x - (radius.x* scale);
	point2.y = this->center.y + (radius.y* scale);
	point2.z = this->center.z + (radius.z* scale);
	this->pVertices.push_back(point2);

	//front bottom left 
	glm::vec3 point3;
	point3.x = this->center.x - (radius.x* scale);
	point3.y = this->center.y - (radius.y* scale);
	point3.z = this->center.z + (radius.z* scale);
	this->pVertices.push_back(point3);

	//BACK POINTS
	//back top right 
	glm::vec3 point4;
	point4.x = this->center.x + (radius.x* scale);
	point4.y = this->center.y + (radius.y* scale);
	point4.z = this->center.z - (radius.z* scale);
	this->pVertices.push_back(point4);

	//back bottom right 
	glm::vec3 point5;
	point5.x = this->center.x + (radius.x* scale);
	point5.y = this->center.y - (radius.y* scale);
	point5.z = this->center.z - (radius.z* scale);
	this->pVertices.push_back(point5);

	//back top left 
	glm::vec3 point6;
	point6.x = this->center.x - (radius.x* scale);
	point6.y = this->center.y + (radius.y* scale);
	point6.z = this->center.z - (radius.z* scale);
	this->pVertices.push_back(point6);

	//back bottom left 
	glm::vec3 point7;
	point7.x = this->center.x - (radius.x* scale);
	point7.y = this->center.y - (radius.y* scale);
	point7.z = this->center.z - (radius.z* scale);
	this->pVertices.push_back(point7);

	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[1], pVertices[3], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[2], pVertices[3], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[4], pVertices[5], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[1], pVertices[5], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[2], pVertices[6], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[4], pVertices[6], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[5], pVertices[1], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[3], pVertices[1], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[3], pVertices[2], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[6], pVertices[2], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[5], pVertices[4], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[6], pVertices[4], glm::vec3(1.0f, 0.0f, 1.0f), -1.0f);
}

unsigned long long AABB_Center_Radius::calculateID(unsigned long meshID, unsigned long triID)
{
	return 0;
}

void AABB_Center_Radius::generateAABBinSpecificLocations(std::vector<glm::vec3> locs, std::vector<float> sizes) {
	for (int i = 0; i < locs.size(); i++)
	{
		generateAABBInSpecificLocation(locs[i], sizes[i]);
	}
}

void  AABB_Center_Radius::generateAABBAtMeshExtents() {
	cMesh tempMesh;
	//clear the old boxes
	this->theInnerBoxes.clear();
	//add some new ones
	g_pVAOManager->lookupMeshFromName(this->meshname, tempMesh);
	//offset the extents with the position
	glm::vec3 tempMaxX = tempMesh.maxX + this->center;
	glm::vec3 tempMinX = tempMesh.minX + this->center;
	glm::vec3 tempMaxZ = tempMesh.maxZ + this->center;
	glm::vec3 tempMinZ = tempMesh.minZ + this->center;

	this->generateAABBInSpecificLocation(tempMinZ, 0.75f);
	this->generateAABBInSpecificLocation(tempMaxZ, 0.75f);
	this->generateAABBInSpecificLocation(tempMaxX, 0.75f);
	this->generateAABBInSpecificLocation(tempMinX, 0.75f);
}

float AABB_Center_Radius::getAverageRadius() {
	float average = (abs(this->radius.x) + abs(this->radius.y) + abs(this->radius.z)) / 3;
	return average;
}

void AABB_Center_Radius::drawCollisionBox(glm::vec3 point,float size, glm::vec3 color) {
	//tempCenter position
	AABB_Center_Radius* innerAABB = new AABB_Center_Radius();
	//FRONT POINTS
	glm::vec3 point0;
	//front top right
	point0.x = point.x + size;
	point0.y = point.y + size;
	point0.z = point.z + size;
	innerAABB->pVertices.push_back(point0);

	//front bottom right
	glm::vec3 point1;
	point1.x = point.x + size;
	point1.y = point.y - size;
	point1.z = point.z + size;
	innerAABB->pVertices.push_back(point1);

	//front top left 
	glm::vec3 point2;
	point2.x = point.x - size;
	point2.y = point.y + size;
	point2.z = point.z + size;
	innerAABB->pVertices.push_back(point2);

	//front bottom left 
	glm::vec3 point3;
	point3.x = point.x - size;
	point3.y = point.y - size;
	point3.z = point.z + size;
	innerAABB->pVertices.push_back(point3);

	//BACK POINTS
	//back top right 
	glm::vec3 point4;
	point4.x = point.x + size;
	point4.y = point.y + size;
	point4.z = point.z - size;
	innerAABB->pVertices.push_back(point4);

	//back bottom right 
	glm::vec3 point5;
	point5.x = point.x + size;
	point5.y = point.y - size;
	point5.z = point.z - size;
	innerAABB->pVertices.push_back(point5);

	//back top left 
	glm::vec3 point6;
	point6.x = point.x - size;
	point6.y = point.y + size;
	point6.z = point.z - size;
	innerAABB->pVertices.push_back(point6);

	//back bottom left 
	glm::vec3 point7;
	point7.x = point.x - size;
	point7.y = point.y - size;
	point7.z = point.z - size;
	innerAABB->pVertices.push_back(point7);

	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[1], innerAABB->pVertices[3], color, 1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[2], innerAABB->pVertices[3], color, 1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[4], innerAABB->pVertices[5], color, 1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[1], innerAABB->pVertices[5], color, 1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[2], innerAABB->pVertices[6], color, 1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[4], innerAABB->pVertices[6], color, 1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[5], innerAABB->pVertices[1], color, 1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[3], innerAABB->pVertices[1], color, 1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[3], innerAABB->pVertices[2], color, 1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[6], innerAABB->pVertices[2], color, 1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[5], innerAABB->pVertices[4], color, 1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[6], innerAABB->pVertices[4], color, 1.0f);
}

void AABB_Center_Radius::generateAABBInSpecificLocation(glm::vec3 point, float size){
	//tempCenter position
	AABB_Center_Radius* innerAABB = new AABB_Center_Radius();
	//FRONT POINTS
	glm::vec3 point0;
	//front top right
	point0.x = point.x + size;
	point0.y = point.y + size;
	point0.z = point.z + size;
	innerAABB->pVertices.push_back(point0);

	//front bottom right
	glm::vec3 point1;
	point1.x = point.x + size;
	point1.y = point.y - size;
	point1.z = point.z + size;
	innerAABB->pVertices.push_back(point1);

	//front top left 
	glm::vec3 point2;
	point2.x = point.x - size;
	point2.y = point.y + size;
	point2.z = point.z + size;
	innerAABB->pVertices.push_back(point2);

	//front bottom left 
	glm::vec3 point3;
	point3.x = point.x - size;
	point3.y = point.y - size;
	point3.z = point.z + size;
	innerAABB->pVertices.push_back(point3);

	//BACK POINTS
	//back top right 
	glm::vec3 point4;
	point4.x = point.x + size;
	point4.y = point.y + size;
	point4.z = point.z - size;
	innerAABB->pVertices.push_back(point4);

	//back bottom right 
	glm::vec3 point5;
	point5.x = point.x + size;
	point5.y = point.y - size;
	point5.z = point.z - size;
	innerAABB->pVertices.push_back(point5);

	//back top left 
	glm::vec3 point6;
	point6.x = point.x - size;
	point6.y = point.y + size;
	point6.z = point.z - size;
	innerAABB->pVertices.push_back(point6);

	//back bottom left 
	glm::vec3 point7;
	point7.x = point.x - size;
	point7.y = point.y - size;
	point7.z = point.z - size;
	innerAABB->pVertices.push_back(point7);

	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[1], innerAABB->pVertices[3], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[2], innerAABB->pVertices[3], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[4], innerAABB->pVertices[5], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[1], innerAABB->pVertices[5], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[2], innerAABB->pVertices[6], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[0], innerAABB->pVertices[4], innerAABB->pVertices[6], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[5], innerAABB->pVertices[1], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[3], innerAABB->pVertices[1], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[3], innerAABB->pVertices[2], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[6], innerAABB->pVertices[2], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[5], innerAABB->pVertices[4], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);
	g_pTheDebugrender->addTriangle(innerAABB->pVertices[7], innerAABB->pVertices[6], innerAABB->pVertices[4], glm::vec3(1.0f, 0.3f, 1.0f),  -1.0f);

	innerAABB->radius.x = size / 2;
	innerAABB->radius.y = size / 2;
	innerAABB->radius.z = size / 2;

	innerAABB->positionOffest = this->center - point;
	innerAABB->center = point;
	this->theInnerBoxes.push_back(innerAABB);
}

void AABB_Center_Radius::adaptVerticesToNewCenter(glm::vec3& newCenter) {
	this->center = newCenter;
	//using the center and radius of the box create the bounding box outside of it
	glm::vec3 point0;

	//FRONT POINTS
	//front top right
	point0.x = this->center.x + (radius.x* this->scale);
	point0.y = this->center.y + (radius.y* this->scale);
	point0.z = this->center.z + (radius.z* this->scale);
	this->pVertices.push_back(point0);

	//front bottom right
	glm::vec3 point1;
	point1.x = this->center.x + (radius.x* this->scale);
	point1.y = this->center.y - (radius.y* this->scale);
	point1.z = this->center.z + (radius.z* this->scale);
	this->pVertices.push_back(point1);

	//front top left 
	glm::vec3 point2;
	point2.x = this->center.x - (radius.x* this->scale);
	point2.y = this->center.y + (radius.y* this->scale);
	point2.z = this->center.z + (radius.z* this->scale);
	this->pVertices.push_back(point2);

	//front bottom left 
	glm::vec3 point3;
	point3.x = this->center.x - (radius.x* this->scale);
	point3.y = this->center.y - (radius.y* this->scale);
	point3.z = this->center.z + (radius.z* this->scale);
	this->pVertices.push_back(point3);

	//BACK POINTS
	//back top right 
	glm::vec3 point4;
	point4.x = this->center.x + (radius.x* this->scale);
	point4.y = this->center.y + (radius.y* this->scale);
	point4.z = this->center.z - (radius.z* this->scale);
	this->pVertices.push_back(point4);

	//back bottom right 
	glm::vec3 point5;
	point5.x = this->center.x + (radius.x* this->scale);
	point5.y = this->center.y - (radius.y* this->scale);
	point5.z = this->center.z - (radius.z* this->scale);
	this->pVertices.push_back(point5);

	//back top left 
	glm::vec3 point6;
	point6.x = this->center.x - (radius.x* this->scale);
	point6.y = this->center.y + (radius.y* this->scale);
	point6.z = this->center.z - (radius.z* this->scale);
	this->pVertices.push_back(point6);

	//back bottom left 
	glm::vec3 point7;
	point7.x = this->center.x - (radius.x* this->scale);
	point7.y = this->center.y - (radius.y* this->scale);
	point7.z = this->center.z - (radius.z* this->scale);
	this->pVertices.push_back(point7);

	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[1], pVertices[3], glm::vec3(1.0f, 0.0f, 1.0f), 0.0f);
	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[2], pVertices[3], glm::vec3(1.0f, 0.0f, 1.0f),  0.0f);
	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[4], pVertices[5], glm::vec3(1.0f, 0.0f, 1.0f),  0.0f);
	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[1], pVertices[5], glm::vec3(1.0f, 0.0f, 1.0f),  0.0f);
	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[2], pVertices[6], glm::vec3(1.0f, 0.0f, 1.0f),  0.0f);
	g_pTheDebugrender->addTriangle(pVertices[0], pVertices[4], pVertices[6], glm::vec3(1.0f, 0.0f, 1.0f),  0.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[5], pVertices[1], glm::vec3(1.0f, 0.0f, 1.0f),  0.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[3], pVertices[1], glm::vec3(1.0f, 0.0f, 1.0f),  0.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[3], pVertices[2], glm::vec3(1.0f, 0.0f, 1.0f),  0.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[6], pVertices[2], glm::vec3(1.0f, 0.0f, 1.0f),  0.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[5], pVertices[4], glm::vec3(1.0f, 0.0f, 1.0f),  0.0f);
	g_pTheDebugrender->addTriangle(pVertices[7], pVertices[6], pVertices[4], glm::vec3(1.0f, 0.0f, 1.0f),  0.0f);
}