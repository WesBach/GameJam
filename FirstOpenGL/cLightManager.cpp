#include "globalOpenGL_GLFW.h"	// glad (gl.h) and GLFW

#include "cLightManager.h"
#include <vector>
#include <sstream>


cLight::cLight()
{
	this->position = glm::vec3(0.0f);

	this->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	this->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	// Colour (xyz), intensity (w)
	this->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// x = constant, y = linear, z = quadratic
	this->attenuation = glm::vec3(0.0f);
	this->attenuation.x = 0.0f;	// Constant atten
	this->attenuation.y = 1.0f;	// Linear
	this->attenuation.z = 0.0f;	// Quadratic

	this->direction = glm::vec3(0.0f);

	// x = type, y = distance cut-off, z angle1, w = angle2
	this->typeParams = glm::vec4(0.0f);

	this->setLightParamType(cLight::POINT);

	// Set distance to infinity
	// (i.e. at this point, the light won't be calculated)
	this->typeParams.y = 1000000.0f;		// Huge number

	this->shaderlocID_position = -1;
	this->shaderlocID_diffuse = -1;
	this->shaderlocID_ambient = -1;
	this->shaderlocID_specular = -1;
	this->shaderlocID_attenuation = -1;
	this->shaderlocID_direction = -1;
	this->shaderlocID_typeParams = -1;

	return;
}

cLightManager::cLightManager()
{

	return;
}

cLightManager::~cLightManager()
{

	return;
}

void cLightManager::CreateLights(int numberOfLights, bool bKeepOldValues /*=true*/)
{
	int howManyLightsToAdd = numberOfLights - (int)this->vecLights.size();
	howManyLightsToAdd = abs(howManyLightsToAdd);
	// Resize the vector
	this->vecLights.resize(howManyLightsToAdd, cLight());

	// If we DON'T want the original values, then clear the old ones
	cLight tempLight;
	for (int index = 0; index != this->vecLights.size(); index++)
	{
		this->vecLights[index] = tempLight;
	}

	return;
}

std::string genUniName(int lightIndex, std::string paramName)
{
	// uniform vec4 myLight[0].position;
	std::stringstream ssUniName;
	ssUniName << "myLight[" << lightIndex << "]." << paramName;
	return ssUniName.str();
}


void cLightManager::LoadShaderUniformLocations(int shaderID)
{
	//struct sLightDesc {
	//vec4 position;
	//vec4 diffuse;
	//vec4 ambient;
	//vec4 specular;		// Colour (xyz), intensity (w)
	//vec4 attenuation;	// x = constant, y = linear, z = quadratic
	//vec4 direction;
	//vec4 typeParams;	// x = type, y = distance cut-off
	//                    // z angle1, w = angle2
	//};
	//const int NUMBEROFLIGHTS = 10;
	//uniform sLightDesc myLight[NUMBEROFLIGHTS];

	for (int index = 0; index != this->vecLights.size(); index++)
	{
		//"myLight[0].position"
		std::string temp = genUniName(index, "position").c_str();
		this->vecLights[index].shaderlocID_position
			= glGetUniformLocation(shaderID, genUniName(index, "position").c_str());

		this->vecLights[index].shaderlocID_diffuse = glGetUniformLocation(shaderID, genUniName(index, "diffuse").c_str());
		this->vecLights[index].shaderlocID_ambient = glGetUniformLocation(shaderID, genUniName(index, "ambient").c_str());
		this->vecLights[index].shaderlocID_specular = glGetUniformLocation(shaderID, genUniName(index, "specular").c_str());
		this->vecLights[index].shaderlocID_attenuation = glGetUniformLocation(shaderID, genUniName(index, "attenuation").c_str());
		this->vecLights[index].shaderlocID_direction = glGetUniformLocation(shaderID, genUniName(index, "direction").c_str());
		this->vecLights[index].shaderlocID_typeParams = glGetUniformLocation(shaderID, genUniName(index, "typeParams").c_str());
	}
	return;
}

void cLightManager::CopyLightInformationToCurrentShader(void)
{
	for (int index = 0; index != this->vecLights.size(); index++)
	{
		cLight& pCurLight = this->vecLights[index];

		glUniform4f(pCurLight.shaderlocID_position,
			pCurLight.position.x,
			pCurLight.position.y,
			pCurLight.position.z,
			1.0f);

		glUniform4f(pCurLight.shaderlocID_diffuse,
			pCurLight.diffuse.r,
			pCurLight.diffuse.g,
			pCurLight.diffuse.b,
			1.0f);

		glUniform4f(pCurLight.shaderlocID_ambient,
			pCurLight.ambient.r,
			pCurLight.ambient.g,
			pCurLight.ambient.b,
			1.0f);

		glUniform4f(pCurLight.shaderlocID_specular,
			pCurLight.specular.r,
			pCurLight.specular.g,
			pCurLight.specular.b,
			pCurLight.specular.w);

		glUniform4f(pCurLight.shaderlocID_attenuation,
			pCurLight.attenuation.x,
			pCurLight.attenuation.y,
			pCurLight.attenuation.z,
			1.0f);

		glUniform4f(pCurLight.shaderlocID_direction,
			pCurLight.direction.x,
			pCurLight.direction.y,
			pCurLight.direction.z,
			1.0f);

		// x = type, y = distance cut-off
		// z angle1, w = angle2		
		glUniform4f(pCurLight.shaderlocID_typeParams,
			pCurLight.typeParams.x,
			pCurLight.typeParams.y,
			pCurLight.typeParams.z,
			pCurLight.typeParams.w);
	}// for ( int index = 0;...

	return;
}


//static 
const float cLight::DEFAULTINFINITEDISTANCE = 10000.0f;
//static 
const float cLight::DEFAULDIFFUSEACCURACYTHRESHOLD = 0.001f;

float cLight::calcApproxDistFromAtten(float targetLightLevel)	// Uses the defaults
{
	return this->calcApproxDistFromAtten(targetLightLevel, DEFAULDIFFUSEACCURACYTHRESHOLD);
}

float cLight::calcApproxDistFromAtten(float targetLightLevel, float accuracy)	// Uses the defaults
{
	return this->calcApproxDistFromAtten(targetLightLevel, accuracy, DEFAULTINFINITEDISTANCE, DEFAULTMAXITERATIONS);
}

float cLight::calcApproxDistFromAtten(float targetLightLevel, float accuracy,
	float infiniteDistance, unsigned int maxIterations /*= DEFAULTMAXITERATIONS = 50*/)
{
	// See if the accuracy being set it too big for the targetLightLevel, unless targetLightLevel is actually zero (0.0f)
	// If it's actually zero, then adjusting the accuracy to a tenth of zero would give... wait for it...
	//	zero, and we would max out the iterations
	if (targetLightLevel != 0.0f)
	{
		if ((accuracy * 10.0f) >= targetLightLevel * 10.0f)
		{	// Adjust the accuracy by a hundreth
			accuracy = targetLightLevel / 10.0f;
		}
	}//if ( targetLightLevel != 0.0f )	

	float targetLightLevelLow = targetLightLevel - accuracy;
	//if ( targetLightLevelLow <= 0.0f )	{ targetLightLevel = 0.0f; }
	float targetLightLevelHigh = targetLightLevel + accuracy;

	// See if we're getting a value at infinite. i.e. at 'infinite distance', is the light level too high already
	if (this->calcDiffuseFromAttenByDistance(DEFAULTINFINITEDISTANCE, accuracy) > targetLightLevelHigh)
	{	// Yes, so we can never get down to this light level
		return DEFAULTINFINITEDISTANCE;
	}

	// There is a light level somewhere between a distance of 0.0 to DEFAULTINFINITEDISTANCE
	// Set initial guesses
	float distanceGuessLow = 0.0f;
	float distanceGuessHigh = DEFAULTINFINITEDISTANCE;

	unsigned int iterationCount = 0;
	while (iterationCount < maxIterations)
	{
		// Pick a distance between the high and low
		float curDistanceGuess = ((distanceGuessHigh - distanceGuessLow) / 2.0f) + distanceGuessLow;
		// 
		float curDiffuseAtGuessDistance = this->calcDiffuseFromAttenByDistance(curDistanceGuess, DEFAULTZEROTHRESHOLD);
		// Could be three possibilities: too low, too high, or in between
		if (curDiffuseAtGuessDistance < targetLightLevelLow)
		{	// Light is too dark, so distance is to HIGH. Reduce and guess again.
			distanceGuessHigh = curDistanceGuess;		// Lower the high limit for the guesses
		}
		else if (curDiffuseAtGuessDistance > targetLightLevelHigh)
		{	// Light is too bright, so distance is to LOW. Increase and guess again
			distanceGuessLow = curDistanceGuess;
		}
		else
		{	// Nailed it! Light level is within range, so return this distance
			return curDistanceGuess;
		}

		iterationCount++;

	}// while ( iterationCount < maxIterations )
	 // If we are here, then we ran out of iterations.
	 // Pick a distance between the low and high
	float distance = (distanceGuessHigh - distanceGuessLow) / 2.0f;

	return distance;
}

//static 
const float cLight::DEFAULTZEROTHRESHOLD = 0.0001f;

float cLight::calcDiffuseFromAttenByDistance(float distance, float zeroThreshold /*= DEFAULTZEROTHRESHOLD*/)
{
	float diffuse = 1.0f;		// Assume full brightness

	float denominator = this->attenuation.x +						// Constant
		this->attenuation.y * distance +			// Linear
		this->attenuation.z * distance * distance;	// Quadratic
													// Is this zero (we don't want a divide by zero, do we)?
	if (denominator <= zeroThreshold)
	{	// Let's just say it's zero, shall we?
		diffuse = 1.0f;
	}
	else
	{
		float atten = 1.0f / denominator;
		diffuse *= atten;
		if (diffuse > 1.0f)
		{
			diffuse = 1.0f;
		}
	}//if ( denominator <= zeroThreshold )
	return diffuse;
}

void cLight::setLightAttenConst(float constAttenuation)
{	//	glm::vec3 attenuation.x = constant
	this->attenuation.x = constAttenuation;
	return;
}

void cLight::setLightAttenLinear(float linearAttenuation)
{	//	glm::vec3 attenuation.y = linear
	this->attenuation.y = linearAttenuation;
	return;
}

void cLight::setLightAttenQuad(float quadraticAttenuation)
{	//	glm::vec3 attenuation.z = quadratic
	this->attenuation.z = quadraticAttenuation;
	return;
}

void cLight::setLightAtten(float constAtten, float linAtten, float quadAtten)
{
	this->attenuation.x = constAtten;
	this->attenuation.y = linAtten;
	this->attenuation.z = quadAtten;
	return;
}

float cLight::getLightAttenConst(void)
{
	return this->attenuation.x;
}

float cLight::getLightAttenLinear(void)
{
	return this->attenuation.y;
}

float cLight::getLightAttenQuad(void)
{
	return this->attenuation.z;
}

//enum eLightType
//{
//	DIRECTIONAL,	// Not implemented
//	POINT,
//	SPOT,			// Not implemented
//	UNKNOWN
//};
void cLight::setLightParamType(eLightType lightType)
{	//	glm::vec4 typeParams.x = type
	// TODO:
	// 		0 = point
	// 		1 = directional
	// 		2 = spot
	// z angle1, w = angle2		- only for spot
	switch (lightType)
	{
	case cLight::POINT:
		this->typeParams.x = 0;	// point
		break;
	case cLight::DIRECTIONAL:
		this->typeParams.x = 1;	// directional
		break;
	case cLight::SPOT:
		this->typeParams.x = 2;	// spot
		break;
	default:
		this->typeParams.x = 0;	// point
								// Choose point...
	};//switch ( lightType )
	return;
}

void cLight::setLightParamDistCutOff(float distanceCutoff)
{	//	glm::vec4 typeParams.y = distance cut-off
	this->typeParams.y = distanceCutoff;
	return;
}

void cLight::setLightParamSpotPrenumAngleInner(float innerPrenumAngle)
{	//	glm::vec4 typeParams.z = angle1
	this->typeParams.z = innerPrenumAngle;
	return;
}

void cLight::setLightParamSpotPrenumAngleOuter(float outerPrenumAngle)
{	//	glm::vec4 typeParams.w = angle2
	this->typeParams.w = outerPrenumAngle;
	return;
}

float cLight::getLightParamSpotPrenumAngleInner(void)
{
	return this->typeParams.z;	//	glm::vec4 typeParams.z = angle1
}

float cLight::getLightParamSpotPrenumAngleOuter(void)	//	glm::vec4 typeParams.w = angle2
{
	return this->typeParams.w; //	glm::vec4 typeParams.w = angle2
}
