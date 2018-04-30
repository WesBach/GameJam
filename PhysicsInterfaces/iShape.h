#pragma once
#include <glm/vec3.hpp>
#include "eShapeType.h"

namespace nPhysics
{
	class iShape
	{
	public:
		virtual ~iShape() {}
		
		eShapeType GetShapeType() { return mShapeType; }

		//accessors for specific shapes
		virtual bool GetSphereRadius(float& radiusOut) { return false; }
		virtual bool GetPlaneNormal(glm::vec3& normalOut) { return false; }
		virtual bool GetPlaneConst(float& planeConstOut) { return false; }
		virtual bool GetCylinderHalfExtents(glm::vec3& halfExtents) { return false; }
		virtual bool GetBoxHalfExtents(glm::vec3& halfExtents) { return false; }
		virtual bool GetConeRadiusAndHeight(float& radius, float& height) { return false; }

	protected:
		iShape(eShapeType shapeType)
			: mShapeType(shapeType)
		{

		}
		iShape() {}
		iShape(const iShape& other) {}
		iShape& operator=(const iShape& other) { return *this; }
	private:
		eShapeType mShapeType;
	};
}