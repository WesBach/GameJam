#ifndef _cEmitter_HG_
#define _cEmitter_HG_

#include <vector>
#include <string>
#include "cParticle.h"
static const unsigned int NUMPARTICLES = 500;

class cEmitter
{
public:
	cEmitter();
	~cEmitter();
	void init(int numParticles, int maxParticleCreatedPerStep,
		glm::vec3 minInitVel, glm::vec3 maxInitVel,
		float minLife, float maxLife,
		glm::vec3 minRangeFromEmitter,
		glm::vec3 maxRangeFromEmitter,
		glm::vec3 acceleration, std::string meshname);
	glm::vec3 position;
	void Update(float deltaTime,glm::vec3 forwardVector);

	void setParticleTextures(std::string name1, std::string name2);
	void setEmitterToActive(bool active);
	bool getActive();
	// returns the number of living particles
	int getLivingParticles(std::vector< cParticle* > &vecParticles);
	unsigned int createEmptyVBO(int size);
	void addInstancedAttributeToParticleVAO(int vao, int vbo, int attribute, int dataSize, int instanceDataLength, int offset);
	void renderParticles(const float& deltaTime, const int& shaderProgram, glm::vec3 playerPosition);
private:
	int m_numParticles;
	std::vector< cParticle* > m_vecParticles;

	glm::vec3 positions[NUMPARTICLES];
	std::string meshName;
	std::string textureName1;
	std::string textureName2;
	bool isActive;

	//Emitter Instance VBO
	int vbo;
	const int DATA_LENGTH = 3;
	// 
	int m_maxParticleCreatedPerStep;
	glm::vec3 m_minInitVel;
	glm::vec3 m_maxInitVel;
	float m_minLife;
	float m_maxLife;
	glm::vec3 m_minRangeFromEmitter;
	glm::vec3 m_maxRangeFromEmitter;
	glm::vec3 m_acceleration;
};

#endif 
