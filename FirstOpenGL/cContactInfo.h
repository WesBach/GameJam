#ifndef _cContactInfo_HG_
#define _cContactInfo_HG_

#include <glm/vec3.hpp>

class cContactInfo {
public:
	cContactInfo();
	~cContactInfo();
	glm::vec3 closestPoint;
	glm::vec3 normal;
	glm::vec3 velocityAtContact;
};

#endif // !_cContactInfo_HG_
