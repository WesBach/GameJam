#ifndef _cPathingNode_HG_
#define _cPathingNode_HG_
#include <glm\vec3.hpp>
#include <vector>

class cPathingNode
{
public:
	cPathingNode();
	~cPathingNode();

	bool isActive;
	float value;

	glm::vec3 position;

	std::vector<cPathingNode*> theConnections;
	unsigned int nodeId;
};
#endif // !_cPathingNode_HG_
