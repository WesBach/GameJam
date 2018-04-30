#ifndef _Physics_HG_
#define _Physics_HG_
#include<vector>
#include<glm\vec3.hpp>


//#include "cGameObject.h"
class cGameObject;
class cPhysTriangle;
class AABB_Center_Radius;

// Sphere-Sphere
// Sphere-Plane
// Sphere-Triangle
// Triangle-Triangle
// Mesh-Mesh
// Sphere-Mesh

struct Plane
{
	glm::vec3 n;
	float d;
};

// Our objects are vectors of pointers, so we might as well pass pointers
bool PenetrationTestSphereSphere( cGameObject* pA, cGameObject* pB );

void ResolveCollisions(std::vector<cGameObject*> theGameObjects);

void CalculateReflection(cGameObject* theObject);

void getXYFromRadiusAndAngle(float& x, float&y, float radius, float angle);

void sphereCollisionResponse(cGameObject* a, cGameObject* b);

//AABB tests
int TestTriangleAABB(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, AABB_Center_Radius& b);

float SqDistPointAABB(glm::vec3& p, AABB_Center_Radius& b);

int TestAABBAABB(AABB_Center_Radius* a, AABB_Center_Radius* b);

int TestAABBPlane(AABB_Center_Radius b, Plane p);

int testPointAABB(glm::vec3 point, AABB_Center_Radius* b);
#endif