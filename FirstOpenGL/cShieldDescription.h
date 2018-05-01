#ifndef _cShieldDescription_HG_
#define _cShieldDescription_HG_
#include <vector>
#include <glm\vec3.hpp>
#include <iRigidBody.h>

class cGameObject;

class cShieldDescription {
public:
	cShieldDescription(int numbodies, glm::vec3 mainBodyPosition);

	std::vector<cGameObject*> pShieldBodies;
};

#endif // !_sShieldDescription_HG_
