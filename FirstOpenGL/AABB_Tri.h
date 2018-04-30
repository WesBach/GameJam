#ifndef _AABB_Tri_HG_
#define _AABB_Tri_HG_
#include "glm\vec3.hpp"

class AABB_Tri{
public:
	AABB_Tri();
	~AABB_Tri();
	AABB_Tri(glm::vec3* verts[3]);
	AABB_Tri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
	AABB_Tri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, unsigned int triID);
	glm::vec3* vertices[3];

	glm::vec3& x();
	glm::vec3& y();
	glm::vec3& z();

	unsigned long long calculateID(glm::vec3 xyzPos);
	inline unsigned long long getId();
private:
	unsigned long long ID;
};
#endif // !_AABB_Tri_HG_

