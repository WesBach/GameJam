#ifndef _sObjectWorldInfo_HG_
#define _sObjectWorldIndo_HG_
#include <glm\vec3.hpp>
#include "AABB_Center_Radius.h"

struct sObjectWorldInfo {
	sObjectWorldInfo() {
		this->position = glm::vec3(0.0f);
		this->orientation = glm::vec3(0.0f);
		this->orientation = glm::vec3(0.0f);
		this->previousPosition = glm::vec3(0.0f);
		this->scale = 1.0f;
	}
	~sObjectWorldInfo() { delete theBoundingBox; }
	glm::vec3 position;
	glm::vec3 previousPosition;
	glm::vec3 orientation;
	glm::vec3 orientation2;		// HACK (will elimiate this with)
	float scale;
	AABB_Center_Radius* theBoundingBox;
};

#endif // !_sObjectWorldInfo_HG_
