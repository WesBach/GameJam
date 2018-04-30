#include "AABB_Tri.h"
#include "Utilities.h"

AABB_Tri::AABB_Tri()
{
}

AABB_Tri::~AABB_Tri()
{
}

AABB_Tri::AABB_Tri(glm::vec3* verts[3])
{
	this->vertices[0] = verts[0];
	this->vertices[1] = verts[1];
	this->vertices[2] = verts[2];
}

AABB_Tri::AABB_Tri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
	this->vertices[0] = &v0;
	this->vertices[1] = &v1;
	this->vertices[2] = &v2;
	this->ID = this->calculateID(getTriangleCentroid(vertices));
}

AABB_Tri::AABB_Tri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, unsigned int triID)
{
	this->vertices[0] = &v0;
	this->vertices[1] = &v1;
	this->vertices[2] = &v2;
	this->ID = triID;
}

unsigned long long AABB_Tri::calculateID(glm::vec3 locationXYZ)
{
	// 4,000,000,000 approx the largest unsigned 32 bit int
	// 16,000,000,000,000,000,000 approx largest unsigned 64 bit
	// If you do xxxxxx,yyyyyyy,zzzzzz where the xyz are the locations of the AABB, 
	//  AND you ONLY make the AABBs positive, then you have a range of
	//  1,000,000 locations. 
	//	So: 
	//   - from 0 to 999,999   if ALL POSITIVE
	//   - from 0 to  99,999   if positive AND negative

	const unsigned long long Z_MULT = 1;
	const unsigned long long Y_MULT = 1000000;
	// With just the numbers, we get an "integral constant overflow", which I think 
	//	is happening because it's using an 32-bit int as const on the rhs, instead
	//	of a 64 bit int...
	const unsigned long long X_MULT = Y_MULT * Y_MULT;	// 1000000 * 1000000;

														// This gives a C4244 warning, even when using an explicit c-style cast. 
														//	...but it's fine with a C++ static_cast. Seriously? Grrr...
	unsigned long long ID = static_cast<unsigned long long>(
		fabs(floor((double)locationXYZ.x) * (double)X_MULT) +		// ?xxxxx,------,------
		fabs(floor((double)locationXYZ.y) * (double)Y_MULT) +		// ------,?yyyyy,------
		fabs(floor((double)locationXYZ.z))); //* Z_MULT;				// ------,------,?zzzzz

											 // You could, if the range isn't large enough:
											 //	- recursively nest these AABBs
											 //  - Divide the world location by the size....
											 //		so (x DIV size), (y DIV size), etc.
											 //unsigned long long ID =
											 //	(fabs(locationXYZ.x / this->size) * X_MULT) +		// ?xxxxx,------,------
											 //	(fabs(locationXYZ.y / this->size) * Y_MULT) +		// ------,?yyyyy,------
											 //	(fabs(locationXYZ.z / this->size)); //* Z_MULT;		// ------,------,?zzzzz

											 // Add the "1" values if negative
	const unsigned long long NEGATIVE_SIGN_MULTIPLIER = 100000;	// 100,000

	const unsigned long long NEG_SIGN_FOR_X = X_MULT * NEGATIVE_SIGN_MULTIPLIER;
	const unsigned long long NEG_SIGN_FOR_Y = Y_MULT * NEGATIVE_SIGN_MULTIPLIER;
	const unsigned long long NEG_SIGN_FOR_Z = Z_MULT * NEGATIVE_SIGN_MULTIPLIER;

	if (locationXYZ.x < 0.0f)
	{	// Add 100,000,000,000,000,000
		ID += NEG_SIGN_FOR_X;
	}
	if (locationXYZ.y < 0.0f)
	{	// Add 100,000,000,000
		ID += NEG_SIGN_FOR_Y;
	}
	if (locationXYZ.z < 0.0f)
	{	// Add 100,000
		ID += NEG_SIGN_FOR_Z;
	}

	return ID;
}

unsigned long long AABB_Tri::getId() {
	return this->ID;
}

glm::vec3& AABB_Tri::x() {
	return *this->vertices[0];
}

glm::vec3& AABB_Tri::y() {
	return *this->vertices[1];
}

glm::vec3& AABB_Tri::z() {
	return *this->vertices[2];
}