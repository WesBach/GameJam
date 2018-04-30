#include "cPhysicalProperties.h"


#include <sstream>


cPhysicalProperties::cPhysicalProperties() 
{
	this->position = this->positionLast = glm::vec3(0.0f);
	this->velocity = glm::vec3(0.0f);
	this->accel = glm::vec3(0.0f);
	this->directedAccel = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->directedVelocity = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	this->m_GameObjectID = 0;		// There shouldn't be an object "0"

	this->integrationUpdateType = cPhysicalProperties::ePhysicsIntegrationUpdateTypes::EXCLUDED_FROM_INTEGRATION;
	this->rigidBodyShape = cPhysicalProperties::eRigidBodyCollisionShapes::IGNORE_EXCLUDE_FROM_COLLISION;

	// Objects world matrix
	this->matWorld = glm::mat4(1.0f);	// identity

	this->setRotationalSpeedEuler(glm::vec3(0.0f));
	this->setOrientationEulerAngles(glm::vec3(0.0f));

	this->mass = 0.0f;			// If zero, assume infinite mass
	this->inverseMass = 0.0f;	// If zero, assume infinite mass

	return;
};

// "set" over-writes the existing values
// "adj" adjusts ("adds"/updates) the angles
void cPhysicalProperties::setOrientationEulerAngles( glm::vec3 newAnglesEuler, bool bIsDegrees /*=false*/ )
{
	if ( bIsDegrees ) { newAnglesEuler = glm::vec3( glm::radians(newAnglesEuler.x), glm::radians(newAnglesEuler.y), glm::radians(newAnglesEuler.z) ); }

	this->qOrientation = glm::quat( glm::vec3( newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z ) );
	return;
}

void cPhysicalProperties::adjOrientationEulerAngles( glm::vec3 adjAngleEuler, bool bIsDegrees /*=false*/ )
{
	if ( bIsDegrees ) { adjAngleEuler = glm::vec3( glm::radians(adjAngleEuler.x), glm::radians(adjAngleEuler.y), glm::radians(adjAngleEuler.z) ); }

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust( adjAngleEuler );	
	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->qOrientation *= rotationAdjust;
	return;
}

void cPhysicalProperties::setOrientationQ( glm::quat newOrientQ )
{
	this->qOrientation = newOrientQ;
	return;
}

void cPhysicalProperties::adjOrientationQ( glm::quat adjOrientQ )
{
	this->qOrientation *= adjOrientQ;
	return;
}

void cPhysicalProperties::setRotationalSpeedEuler( glm::vec3 newRotSpeedEuler, bool bIsDegrees /*=false*/ )
{
	if ( bIsDegrees ) { glm::vec3( glm::radians(newRotSpeedEuler.x), glm::radians(newRotSpeedEuler.y), glm::radians(newRotSpeedEuler.z) ); }

	this->qRotationalSpeed = glm::quat( newRotSpeedEuler );
	return;
}

void cPhysicalProperties::adjRotationalSpeedEuler( glm::vec3 adjRotSpeedEuler, bool bIsDegrees /*=false*/ )
{
	if ( bIsDegrees ) { glm::vec3( glm::radians(adjRotSpeedEuler.x), glm::radians(adjRotSpeedEuler.y), glm::radians(adjRotSpeedEuler.z) ); }

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationSpeedAdjust( adjRotSpeedEuler );	
	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->qRotationalSpeed *= rotationSpeedAdjust;
	return;
}

void cPhysicalProperties::setRotationalSpeedQ( glm::quat newRotSpeedQ )
{
	this->qRotationalSpeed = newRotSpeedQ;
	return;
}

void cPhysicalProperties::adjRotationalSpeedQ( glm::quat adjRotSpeedQ )
{
	this->qRotationalSpeed *= adjRotSpeedQ;
	return;
}

void cPhysicalProperties::setAcceleration(glm::vec3 accel) {
	this->accel = accel;
}

void cPhysicalProperties::setVelocity(glm::vec3 vel) {
	this->velocity = vel;

}

std::string cPhysicalProperties::DEBUG_getString_position(void)
{
	std::stringstream ss;
	ss << "position: " << this->position.x << ", " << this->position.y << ", " << this->position.z;
	return ss.str();
}

std::string cPhysicalProperties::DEBUG_getString_positionLast(void)
{
	std::stringstream ss;
	ss << "positionLast: " << this->positionLast.x << ", " << this->positionLast.y << ", " << this->positionLast.z;
	return ss.str();
}

std::string cPhysicalProperties::DEBUG_getString_velocity(void)
{
	std::stringstream ss;
	ss << "velocity: " << this->velocity.x << ", " << this->velocity.y << ", " << this->velocity.z;
	return ss.str();
}

std::string cPhysicalProperties::DEBUG_getString_accel(void)
{
	std::stringstream ss;
	ss << "accel: " << this->accel.x << ", " << this->accel.y << ", " << this->accel.z;
	return ss.str();
}

std::string cPhysicalProperties::DEBUG_getString_directedVelocity(void)
{
	std::stringstream ss;
	ss << "directedVelocity: " << this->directedVelocity.x << ", " << this->directedVelocity.y << ", " << this->directedVelocity.z;
	return ss.str();
}

std::string cPhysicalProperties::DEBUG_getString_directedAccel(void)
{
	std::stringstream ss;
	ss << "directedAccel: " << this->directedAccel.x << ", " << this->directedAccel.y << ", " << this->directedAccel.z;
	return ss.str();
}
