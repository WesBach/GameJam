#ifndef _cParticle_HG_
#define _cParticle_HG_

#include <glm/vec3.hpp>

class cParticle
{
public:
	cParticle()
	{
		lifetime = 0.0f;
		pos = glm::vec3(0.0f);
		vel = glm::vec3(0.0f);
		color = glm::vec3(0.0f);
		scale = 0.0f;
	};
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 color;
	float lifetime;
	float scale;

};

#endif
