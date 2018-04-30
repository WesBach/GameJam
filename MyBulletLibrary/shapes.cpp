#include "Shapes.h"

namespace nPhysics {

	//sphere shape 1 arg constructor
	cBulletSphereShape::cBulletSphereShape(float radius)
		: iShape(SHAPE_TYPE_SPHERE)
		, mRadius(radius){

	}

	//sphere shape default constructor
	cBulletSphereShape::cBulletSphereShape()
		: iShape(SHAPE_TYPE_SPHERE){

	}

	//sohere shape copy constructor
	cBulletSphereShape::cBulletSphereShape(const cBulletSphereShape& other)
		: iShape(SHAPE_TYPE_SPHERE){

	}

	//sphere = operator
	cBulletSphereShape& cBulletSphereShape::operator=(const cBulletSphereShape& other){
		return *this;
	}
	//sphere shape destructor
	cBulletSphereShape::~cBulletSphereShape(){

	}

	//sphere radius accessor
	bool cBulletSphereShape::GetSphereRadius(float& radiusOut){
		radiusOut = mRadius;
		return true;
	}

	//plane shape 2 arg constructor
	cBulletPlaneShape::cBulletPlaneShape(const glm::vec3& normal, float planeConst)
		: iShape(SHAPE_TYPE_PLANE)
		, mNormal(normal)
		, mPlaneConst(planeConst){

	}

	//plane default constructor
	cBulletPlaneShape::cBulletPlaneShape()
		: iShape(SHAPE_TYPE_PLANE){

	}

	//plane copy constructor
	cBulletPlaneShape::cBulletPlaneShape(const cBulletPlaneShape& other)
		: iShape(SHAPE_TYPE_PLANE){

	}

	//plane = operator
	cBulletPlaneShape& cBulletPlaneShape::operator=(const cBulletPlaneShape& other)	{
		return *this;
	}

	//plane destructor
	cBulletPlaneShape::~cBulletPlaneShape(){

	}
	
	//plane normal accessor
	bool cBulletPlaneShape::GetPlaneNormal(glm::vec3& normalOut){
		normalOut = mNormal;
		return true;
	}

	//plane const accessor
	bool cBulletPlaneShape::GetPlaneConst(float& planeConstOut){
		planeConstOut = mPlaneConst;
		return true;
	}

	//Cylinder shape 1 arg constructor
	cBulletCylinderShape::cBulletCylinderShape(const glm::vec3& halfExtents)
		:iShape(SHAPE_TYPE_CYLINDER)
		,mHalfExtents(halfExtents){

	}

	//cylinder shape default constructor
	cBulletCylinderShape::cBulletCylinderShape()
		: iShape(SHAPE_TYPE_CYLINDER) {

	}

	//cylinder copy constructor
	cBulletCylinderShape::cBulletCylinderShape(const cBulletCylinderShape& other)
		: iShape(SHAPE_TYPE_CYLINDER) {

	}

	//cylinder destructor
	cBulletCylinderShape::~cBulletCylinderShape(){

	}

	//cylinder half extents accessor 
	bool cBulletCylinderShape::GetCylinderHalfExtents(glm::vec3& halfExtentsOut) {
		halfExtentsOut = this->mHalfExtents;
		return true;
	}

	//cylinder = operator
	cBulletCylinderShape& cBulletCylinderShape::operator=(const cBulletCylinderShape& other) {
		return *this;
	}

	//Box shape 1 arg constructor
	cBulletBoxShape::cBulletBoxShape(const glm::vec3& boxHalfExtents)
		:iShape(SHAPE_TYPE_BOX)
		,mHalfExtents(boxHalfExtents){

	}
	//Box shape default constructor
	cBulletBoxShape::cBulletBoxShape()
		: iShape(SHAPE_TYPE_BOX) {

	}

	//box shape copy constructor
	cBulletBoxShape::cBulletBoxShape(const cBulletBoxShape& other)
		: iShape(SHAPE_TYPE_BOX) {

	}
	//box shape destructor
	cBulletBoxShape::~cBulletBoxShape() {

	}

	//box shape half extents accessor
	bool cBulletBoxShape::GetBoxHalfExtents(glm::vec3 & halfExtentsOut)	{
		halfExtentsOut = this->mHalfExtents;
		return false;
	}
	//box shape = operator
	cBulletBoxShape& cBulletBoxShape::operator=(const cBulletBoxShape& other) {
		return *this;
	}

	//cone shape 2 arg constructor
	cBulletConeShape::cBulletConeShape(const float& radius, const float& height) 
		:iShape(SHAPE_TYPE_CONE)
		,mRadius(radius)
		,mHeight(height) {

	}
	//cone shape destructor
	cBulletConeShape::~cBulletConeShape() {

	}
	//cone shape radius and height accessor
	bool cBulletConeShape::GetConeRadiusAndHeight(float& radius, float& height) {
		radius = this->mRadius;
		height = this->mHeight;
		return true;
	}

	//cone shape default constructor
	cBulletConeShape::cBulletConeShape() :iShape(SHAPE_TYPE_CONE) {

	}
	//cone shape copy constructor
	cBulletConeShape::cBulletConeShape(const cBulletBoxShape& other):iShape(SHAPE_TYPE_CONE) {

	}
	//cone shape = operator
	cBulletConeShape& cBulletConeShape::operator=(const cBulletConeShape& other) {
		return *this;
	}

}