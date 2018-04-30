#include "cPathingNode.h"

cPathingNode::cPathingNode() {
	this->isActive = false;
	this->value = 0.0f;
	this->position = glm::vec3(0.0f);
}

cPathingNode::~cPathingNode() {

}