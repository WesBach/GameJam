#ifndef _Shapes_HG_
#define _Shapes_HG_
#include <iShape.h>
#include <bullet\btBulletDynamicsCommon.h>

namespace nPhysics {

	//Shere shape
	class cBulletSphereShape : public iShape{
	public:
		cBulletSphereShape(float radius);
		virtual ~cBulletSphereShape();
		virtual bool GetSphereRadius(float& radiusOut);

	private:
		cBulletSphereShape();
		cBulletSphereShape(const cBulletSphereShape& other);
		cBulletSphereShape& operator=(const cBulletSphereShape& other);
		float mRadius;
	};

	//Plane shape
	class cBulletPlaneShape : public iShape{
	public:
		cBulletPlaneShape(const glm::vec3& normal, float planeConst);
		virtual ~cBulletPlaneShape();
		virtual bool GetPlaneNormal(glm::vec3& normalOut);
		virtual bool GetPlaneConst(float& planeConstOut);

	private:
		cBulletPlaneShape();
		cBulletPlaneShape(const cBulletPlaneShape& other);
		cBulletPlaneShape& operator=(const cBulletPlaneShape& other);
		glm::vec3 mNormal;
		float mPlaneConst;
	};

	//Cylinder shape
	class cBulletCylinderShape : public iShape{
	public:
		cBulletCylinderShape(const glm::vec3& halfExtents);
		virtual ~cBulletCylinderShape();
		virtual bool GetCylinderHalfExtents(glm::vec3& halfExtentsOut);
	private:
		cBulletCylinderShape();
		cBulletCylinderShape(const cBulletCylinderShape& other);
		cBulletCylinderShape& operator=(const cBulletCylinderShape& other);
		glm::vec3 mHalfExtents;
	};

	//Box shape
	class cBulletBoxShape : public iShape{
	public:
		cBulletBoxShape(const glm::vec3& boxHalfExtents);
		virtual ~cBulletBoxShape();
		virtual bool GetBoxHalfExtents(glm::vec3& halfExtentsOut);
	private:
		cBulletBoxShape();
		cBulletBoxShape(const cBulletBoxShape& other);
		cBulletBoxShape& operator=(const cBulletBoxShape& other);
		glm::vec3 mHalfExtents;
	};

	//Cone shape
	class cBulletConeShape : public iShape
	{
	public:
		cBulletConeShape(const float& radius, const float& height);
		virtual ~cBulletConeShape();
		virtual bool GetConeRadiusAndHeight(float& radius,float& height);
	private:
		cBulletConeShape();
		cBulletConeShape(const cBulletBoxShape& other);
		cBulletConeShape& operator=(const cBulletConeShape& other);
		float mHeight;
		float mRadius;
	};

}


#endif
