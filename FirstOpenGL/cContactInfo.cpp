#include"cContactInfo.h"

cContactInfo::cContactInfo() {
	this->closestPoint = glm::vec3(0.0f);
	this->normal = glm::vec3(0.0f);
	this->velocityAtContact = glm::vec3(0.0f);
}
cContactInfo::~cContactInfo() {
	return;
}