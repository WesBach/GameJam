#include "Physics.h"
#include "cGameObject.h"
#include "cTriangle.h"
#include "map"
#include <glm/glm.hpp>		// glm::distance

bool PenetrationTestSphereSphere( cGameObject* pA, cGameObject* pB )
{
	// If the distance between the two sphere is LT the sum or the radii,
	//	they are touching or penetrating

	float totalRadii = pA->radius + pB->radius;

	// The Pythagorean distance 
	float distance = glm::distance( pA->position, pB->position );

	if ( distance <= totalRadii )
	{
		return true;
	}

	return false;
}

void sphereCollisionResponse(cGameObject* a, cGameObject* b)
{
	glm::vec3 U1x, U1y, U2x, U2y, V1x, V1y, V2x, V2y;

	float m1, m2, x1, x2;
	glm::vec3 v1temp, v1, v2, v1x, v2x, v1y, v2y, x(a->position - b->position);

	glm::normalize(x);
	v1 = a->vel;
	x1 = dot(x,v1);
	v1x = x * x1;
	v1y = v1 - v1x;
	m1 = 1.0f; //mass of 1

	x = x*-1.0f;
	v2 = b->vel;
	x2 = dot(x,v2);
	v2x = x * x2;
	v2y = v2 - v2x;
	m2 = 1.0f; //mass of 1

	//set the position of the spheres to their previous non contact positions to unstick them.
	a->position = a->previousPosition;
	b->position = b->previousPosition;
	a->vel = glm::vec3(v1x*(m1 - m2) / (m1 + m2) + v2x*(2 * m2) / (m1 + m2) + v1y) /4.0f;
	b->vel = glm::vec3(v1x*(2 * m1) / (m1 + m2) + v2x*(m2 - m1) / (m1 + m2) + v2y) /4.0f;
}

// inline?? 
glm::vec3 cPhysTriangle::ClosestPtPointTriangle(glm::vec3* p)
{
	return this->ClosestPtPointTriangle( p, &this->vertex[0], 
										    &this->vertex[1], 
										    &this->vertex[2] );
}

//need to update the position of the sphere and store it in center in game loop or physics loop
//TO DO: Figure it out
int cPhysTriangle::TestSphereTriangle(cGameObject* s, glm::vec3* a, glm::vec3* b, glm::vec3* c, glm::vec3 &p)
{
	// Find point P on triangle ABC closest to sphere center
	p = ClosestPtPointTriangle(&(s->position), a, b, c);

	// Sphere and triangle intersect if the (squared) distance from sphere
	// center to point p is less than the (squared) sphere radius
	glm::vec3 v = p - s->position;
	return glm::dot(v, v) <= s->radius * s->radius;
}

glm::vec3 cPhysTriangle::ClosestPtPointTriangle(glm::vec3* p, glm::vec3* a, glm::vec3* b, glm::vec3* c)
{
    // Check if P in vertex region outside A
    glm::vec3 ab = (*b) - (*a);
    glm::vec3 ac = (*c) - (*a);
    glm::vec3 ap = (*p) - (*a);
    float d1 = glm::dot(ab, ap);
    float d2 = glm::dot(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f) return *a; // barycentric coordinates (1,0,0)

    // Check if P in vertex region outside B
    glm::vec3 bp = (*p) - (*b);
    float d3 = glm::dot(ab, bp);
    float d4 = glm::dot(ac, bp);
    if (d3 >= 0.0f && d4 <= d3) return *b; // barycentric coordinates (0,1,0)

    // Check if P in edge region of AB, if so return projection of P onto AB
    float vc = d1*d4 - d3*d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        float v = d1 / (d1 - d3);
        return *a + v * ab; // barycentric coordinates (1-v,v,0)
    }

    // Check if P in vertex region outside C
    glm::vec3 cp = *p - *c;
    float d5 = glm::dot(ab, cp);
    float d6 = glm::dot(ac, cp);
    if (d6 >= 0.0f && d5 <= d6) return *c; // barycentric coordinates (0,0,1)

    // Check if P in edge region of AC, if so return projection of P onto AC
    float vb = d5*d2 - d1*d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        float w = d2 / (d2 - d6);
        return *a + w * ac; // barycentric coordinates (1-w,0,w)
    }

    // Check if P in edge region of BC, if so return projection of P onto BC
    float va = d3*d6 - d5*d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return *b + w * (c - b); // barycentric coordinates (0,1-w,w)
    }

    // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    return *a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
}

int TestAABBAABB(AABB_Center_Radius* a, AABB_Center_Radius* b)
{
	int r;
	r = a->radius.x + b->radius.x; 
	if ((unsigned int)(a->center.x - b->center.x + r) > r + r) return 0;
	r = a->radius.y + b->radius.y; 
	if ((unsigned int)(a->center.y - b->center.y + r) > r + r) return 0;
	r = a->radius.z + b->radius.z; 
	if ((unsigned int)(a->center.z - b->center.z + r) > r + r) return 0;
	return 1;
}

int testPointAABB(glm::vec3 point, AABB_Center_Radius* b)
{
	bool isWithinPosX = false;
	bool isWithinNegX = false;

	if (point.x > b->center.x  && point.x < b->center.x + b->radius.x)
		isWithinPosX = true;

	if (point.x < b->center.x  && point.x > b->center.x - b->radius.x)
		isWithinNegX = true;

	if (isWithinNegX == false || isWithinPosX == false)
	{
		return 0;
	}

	bool isWithinPosY = false;
	bool isWithinNegY = false;

	if (point.y > b->center.y  && point.y < b->center.y + b->radius.y)
		isWithinPosY = true;

	if (point.y < b->center.y  && point.y > b->center.y - b->radius.y)
		isWithinNegY = true;

	if (isWithinPosY == false || isWithinNegY == false)
	{
		return 0;
	}

	bool isWithinPosZ = false;
	bool isWithinNegZ = false;

	if (point.z > b->center.z  && point.z < b->center.z + b->radius.z)
		isWithinPosZ = true;

	if (point.z < b->center.z  && point.z > b->center.z - b->radius.z)
		isWithinNegZ = true;

	if (isWithinPosZ == false || isWithinNegZ == false)
	{
		return 0;
	}

	return 1;
}

int TestTriangleAABB(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, AABB_Center_Radius& b)
{
	float p0, p1, p2, r;

	// Compute box center and extents (if not already given in that format)

	//glm::vec3 c = (b.min + b.max) * 0.5f;
	//float e0 = (b.max.x – b.min.x) * 0.5f;
	//float e1 = (b.max.y – b.min.y) * 0.5f;
	//float e2 = (b.max.z – b.min.z) * 0.5f;

	glm::vec3 c = b.center;
	float e0 = b.radius.x * 2.0f;
	float e1 = b.radius.y * 2.0f;
	float e2 = b.radius.z * 2.0f;

	// Translate triangle as conceptually moving AABB to origin
	v0 -= c;
	v1 -= c;
	v2 -= c;

	// Compute edge vectors for triangle
	glm::vec3 f0 = v1 - v0, f1 = v2 - v1, f2 = v0 - v2;

	// Test axes a00..a22 (category 3)
	// Test axis a00
	p0 = v0.z*v1.y - v0.y*v1.z;
	p2 = v2.z*(v1.y - v0.y) - v2.z*(v1.z - v0.z);
	r = e1 * abs(f0.z) + e2 * abs(f0.y);
	if (glm::max(-glm::max(p0, p2), glm::min(p0, p2)) > r)
		return 0; // Axis is a separating axis
	// Repeat similar tests for remaining axes a01..a22
	
	// Test the three axes corresponding to the face normals of AABB b (category 1).
	// Exit if...
	// ... [-e0, e0] and [min(v0.x,v1.x,v2.x), max(v0.x,v1.x,v2.x)] do not overlap
	float max0 = glm::max(glm::max(v0.x, v1.x), v2.x);
	float min0 = glm::min(glm::min(v0.x, v1.x), v2.x);
	if (max0 < -e0 || min0 > e0) return 0;

	float max1 = glm::max(glm::max(v0.y, v1.y), v2.y);
	float min1 = glm::min(glm::min(v0.y, v1.y), v2.y);
	// ... [-e1, e1] and [min(v0.y,v1.y,v2.y), max(v0.y,v1.y,v2.y)] do not overlap
	if (max1 < -e1 || min1 > e1) return 0;

	float max2 = glm::max(glm::max(v0.z, v1.z), v2.z);
	float min2 = glm::min(glm::min(v0.z, v1.z), v2.z);
	// ... [-e2, e2] and [min(v0.z,v1.z,v2.z), max(v0.z,v1.z,v2.z)] do not overlap
	if ( max2 < -e2 || min2 > e2) return 0;

	//// Test separating axis corresponding to triangle face normal (category 2)
	Plane p;
	p.n = glm::cross(f0, f1);
	p.d = glm::dot(p.n, v0);
	return TestAABBPlane(b, p);
}

// Test if AABB b intersects plane p
int TestAABBPlane(AABB_Center_Radius b, Plane p)
{
	// These two lines not necessary with a (center, extents) AABB representation
	glm::vec3 c = b.center; // Compute AABB center
	glm::vec3 e = b.radius * 2.0f; // Compute positive extents

	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	float r = e[0] * abs(p.n[0]) + e[1] * abs(p.n[1]) + e[2] * abs(p.n[2]);
	// Compute distance of box center from plane
	float s = glm::dot(p.n, c) - p.d;
	// Intersection occurs when distance s falls within [-r,+r] interval
	return abs(s) <= r;
}

// Given point p, return the point q on or in AABB b, that is closest to p
//void ClosestPtPointAABB(glm::vec3 p, AABB_Center_Radius b, glm::vec3 &q)
//{
//	// For each coordinate axis, if the point coordinate value is
//	// outside box, clamp it to the box, else keep it as is
//	for (int i = 0; i < 3; i++) {
//		float v = p[i];
//
//		if (v < b.min[i]) 
//			v = b.min[i]; // v = max(v, b.min[i])
//
//		if (v > b.max[i]) 
//			v = b.max[i]; // v = min(v, b.max[i])
//		q[i] = v;
//	}
//}

// Computes the square distance between a point p and an AABB b
float SqDistPointAABB(glm::vec3& p, AABB_Center_Radius& b)
{
	float sqDist = 0.0f;
	for (int i = 0; i < 3; i++) {
		// For each axis count any excess distance outside box extents
		float v = p[i];
		if (v < b.radius[i]) 
			sqDist += (b.radius[i] - v) * (b.radius[i] - v);
		if (v > b.radius[i]) 
			sqDist += (v - b.radius[i]) * (v - b.radius[i]);
	}
	return sqDist;
}

//
//// Transform AABB a by the matrix m and translation t,
//// find maximum extents, and store result into AABB b.
//void UpdateAABB(AABB a, float m[3][3], float t[3], AABB &b)
//{
//	for (int i = 0; i < 3; i++) {
//		b.center[i] = t[i];
//		b.radius[i] = 0.0f;
//		for (int j = 0; j < 3; j++) {
//			b.center[i] += m[i][j] * a.center[j];
//			b.radius[i] += glm::abs(m[i][j]) * a.radius[j];
//		}
//	}
//}

////checks to see if any of the game objects have contact points
//void ResolveCollisions(std::vector<cGameObject*> theGameObjects) {
//	for (int objectIndex = 0; objectIndex != theGameObjects.size(); objectIndex++)
//	{
//		//if the object has contact points
//		if (theGameObjects[objectIndex]->contactPoints.size() > 0)
//		{
//			CalculateReflection(theGameObjects[objectIndex]);
//		}
//	}
//}
//
////calculates the reflection of the object that contacted terrain
//void CalculateReflection(cGameObject* theObject) {
//
//	theObject->position = theObject->previousPosition;
//	glm::vec3 tempVecNormal = glm::normalize(theObject->contactPoints[0]->normal);
//	theObject->vel = glm::reflect(theObject->contactPoints[0]->velocityAtContact, tempVecNormal) * 0.98f;
//	//clear the vector of contact points for that object
//	theObject->contactPoints.clear();
//}