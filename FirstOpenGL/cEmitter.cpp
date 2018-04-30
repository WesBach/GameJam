#include "cEmitter.h"
#include "globalOpenGL_GLFW.h"
#include "RenderUtilities.h"
#include "cVAOMeshManager.h"
#include "cMesh.h"

extern cVAOMeshManager* g_pVAOManager;
extern cGameObject tempObject;
cEmitter::cEmitter()
{
	// Maybe set some defaults?? 
	// (Or just be careful, right?)
	this->m_numParticles = 0;
	this->m_maxParticleCreatedPerStep = 0;
	this->m_minLife = 0.0f;
	this->m_maxLife = 0.0f;
	// 
	this->m_maxInitVel = glm::vec3(0.0f);
	this->m_minInitVel = glm::vec3(0.0f);
	this->m_minRangeFromEmitter = glm::vec3(0.0f);
	this->m_maxRangeFromEmitter = glm::vec3(0.0f);
	this->isActive = false;
	return;
}

cEmitter::~cEmitter()
{
	// Delete the particles
	return;
}

double getRandInRange( double min, double max )
{

	double value = min + static_cast <double> (rand()) / ( static_cast <double> (RAND_MAX/(max-min)));
	return value;
}

// Call this only to start of if you want to change the initial values
// (NOT to be called every frame or anything)
// (Also, call IN ADVANCE of "expensive" operations, like explosions, 
//  or other things that require a lot of particles)
void cEmitter::init(
	int numParticles, int maxParticleCreatedPerStep,
	glm::vec3 minInitVel, glm::vec3 maxInitVel,
	float minLife, float maxLife,
	glm::vec3 minRangeFromEmitter,
	glm::vec3 maxRangeFromEmitter, 
	glm::vec3 acceleration,std::string meshname )
{
	this->meshName = meshname;
	// Do I already have this many particles in my vector? 
	int numParticlesToCreate = numParticles - this->m_vecParticles.size();
	// If we DON'T have enough, then create some
	for (int count = 0; count <= numParticlesToCreate; count++)
	{	// 
		this->m_vecParticles.push_back( new cParticle() );
	}
	// At this point, we have enough particles

	// Reset all the particles to the default position and life
	this->m_numParticles = numParticles;
	for (int index = 0; index != this->m_numParticles; index++)
	{	// reset ONLY the lifetime
		this->m_vecParticles[index]->lifetime = 0.0f;	// He's dead, Jim.
	}

	// Save all the init values we passed
	this->m_maxParticleCreatedPerStep = maxParticleCreatedPerStep;
	this->m_minInitVel = minInitVel;
	this->m_maxInitVel = maxInitVel;
	this->m_minLife = minLife;
	this->m_maxLife = maxLife;
	this->m_minRangeFromEmitter = minRangeFromEmitter;
	this->m_maxRangeFromEmitter = maxRangeFromEmitter;
	this->m_acceleration = acceleration;

	//creates the instance data vbo
	//10 because we are only sending position, color, normal
	this->vbo = createEmptyVBO(NUMPARTICLES);
	sVAOInfo VAODrawInfo;
	g_pVAOManager->lookupVAOFromName(meshname, VAODrawInfo);
	//addInstancedAttributeToParticleVAO(VAODrawInfo.VAO_ID, vbo, 7, DATA_LENGTH, DATA_LENGTH, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(VAODrawInfo.VAO_ID);

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, /*instanceDataLength * sizeof(float)*/0, (void*)0);
	glVertexAttribDivisor(7, 1);
	glBindVertexArray(0);
	glDisableVertexAttribArray(7);

	return;
}

int cEmitter::getLivingParticles(std::vector< cParticle* > &vecParticles)
{
	// Resize if too small	
	if (vecParticles.size() < this->m_numParticles)
	{
		// Resize and fill with NULL (or whatever)
		vecParticles.resize(this->m_numParticles, NULL);
	}
	// Copy all the alive particles
	int indexAliveParticles = 0;
	for (int index = 0; index != this->m_numParticles; index++)
	{
		// Is it alive?!
		if (this->m_vecParticles[index]->lifetime > 0.0f)
		{	// yes
			vecParticles[indexAliveParticles] = this->m_vecParticles[index];
			indexAliveParticles++;
		}
	}//for (int index

	// 1 more than the number of alive particles
	// HACK: o-rama!
	//indexAliveParticles -= 50;
	if (indexAliveParticles < 0) indexAliveParticles = 0;
	//
	return indexAliveParticles;
}

void cEmitter::Update(float deltaTime,glm::vec3 forwardVector)
{
	// 1. Calculate how many particles from m_maxParticleCreatedPerStep
	// 2. Find 'dead' particles in the vector
	// 3. IF you find them, 'create' new particles (copy init values)
	
	int particlesToStillMake = this->m_maxParticleCreatedPerStep;

	// Scan through the vector, looking for "dead" particles
	for (int index = 0; 
		 (index != this->m_numParticles) && (particlesToStillMake > 0);
		 index++)
	{
		// Is this particle 'dead'
		if (this->m_vecParticles[index]->lifetime <= 0.0f)
		{	// "create" a particle			
			this->m_vecParticles[index]->lifetime = getRandInRange( this->m_minLife, this->m_maxLife );
			this->m_vecParticles[index]->pos = this->position;
			this->m_vecParticles[index]->vel.x = getRandInRange( this->m_minRangeFromEmitter.x, this->m_maxRangeFromEmitter.x );
			this->m_vecParticles[index]->vel.y = getRandInRange( this->m_minRangeFromEmitter.y, this->m_maxRangeFromEmitter.y );
			this->m_vecParticles[index]->vel.z = getRandInRange( this->m_minRangeFromEmitter.z, this->m_maxRangeFromEmitter.z );
			// Update the count
			particlesToStillMake--;
		}
	}//for (int index

	// 4. Perform integration step (aka Euler, or use physics, etc.)
	//    (ALSO: decrease the 'life' of the particle by deltaTime)
	for (int index = 0; index != this->m_numParticles; index++)
//	for (int index = 0; index != this->m_vecParticles.size()-1; index++)
	{
		// Forward explicit Euler...		
		// Velocity comes from accleration 
		this->m_vecParticles[index]->vel += (this->m_acceleration * deltaTime);

		// Position comes from velocity
		this->m_vecParticles[index]->pos +=
                         (this->m_vecParticles[index]->vel * deltaTime);

		// Update the lifetime
		this->m_vecParticles[index]->lifetime -= deltaTime;
	}

	return;
}

unsigned int cEmitter::createEmptyVBO(int size) {
	unsigned int vbo;
	//generate the vbo id
	glGenBuffers(1, &(vbo));
	//bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//no give the size but no data
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 3 * size, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

void cEmitter::addInstancedAttributeToParticleVAO(int vao, int vbo, int attribute, int dataSize, int instanceDataLength, int offset) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, /*instanceDataLength * sizeof(float)*/0, (void*)0);
	glVertexAttribDivisor(attribute, 1);

	glBindVertexArray(0);
}

void cEmitter::renderParticles(const float& deltaTime,const int& shaderProgram,glm::vec3 playerPosition) {

	//get the active particles for each active emitter and draw that amount of particles to the screen
	tempObject.meshName = this->meshName;
	tempObject.bIsWireFrame = false;
	tempObject.bIsSkyBoxObject = false;
	//make slightly transparent
	tempObject.diffuseColour = glm::vec4(1.f, 1.f, 1.f, 1.0f);
	tempObject.scale = 0.25;/*getRandInRange(0.1f, 1.0f);*/
	tempObject.position = this->position;
	//populate the object data
	float textureBlend = getRandInRange(0.0f, 1.0f);
	float texBlend2 = 1.0f - textureBlend;
	//set the textures with random blend values
	tempObject.vecMehs2DTextures.push_back(sTextureBindBlendInfo(textureName1, textureBlend));
	tempObject.vecMehs2DTextures.push_back(sTextureBindBlendInfo(textureName2, texBlend2));

	std::vector<cParticle*> activeParticles;
	int numActiveParticles = this->getLivingParticles(activeParticles);
	std::vector<cGameObject*> theParticlesToDraw;

	//set the position offsets to 0.f 
	for (int i = 0; i < NUMPARTICLES; i++) {
		positions[i] = glm::vec3(0.f);
	}

	for (int particleIndex = 0; particleIndex < numActiveParticles; particleIndex++) {
		//particle position from the emitter
		this->positions[particleIndex] =  activeParticles[particleIndex]->pos - this->position ;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numActiveParticles, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * numActiveParticles, positions);

	//draw the object and it's copies
	DrawObject(&tempObject, shaderProgram, deltaTime, true, numActiveParticles);
	tempObject.vecMehs2DTextures.clear();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cEmitter::setParticleTextures(std::string name1, std::string name2) {
	//set the texture names
	this->textureName1 = name1;
	this->textureName2 = name2;
}

void cEmitter::setEmitterToActive(bool active) {
	this->isActive = active;
}

bool cEmitter::getActive() {
	return this->isActive;
}