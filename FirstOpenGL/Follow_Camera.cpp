#include "Camera.h"

float smoothstep(float edge0, float edge1, float x);
float smootherstep(float edge0, float edge1, float x);
float clamp(float x, float lowerlimit, float upperlimit);

void Camera::Follow_SetOrUpdateTarget(glm::vec3 target)
{
	this->target = target;
	return;
}
void Camera::Follow_SetIdealCameraLocation(glm::vec3 relativeToTarget)
{
	this->follow_idealCameraLocationRelToTarget = relativeToTarget;
	return;
}
void Camera::Follow_SetMaxFollowSpeed(float speed)
{
	this->follow_max_speed = speed;
	return;
}

void Camera::Follow_SetDistanceMaxSpeed(float distanceToTarget)
{
	this->follow_distance_max_speed = distanceToTarget;
	return;
}

void Camera::Follow_SetDistanceMinSpeed(float distanceToTarget)
{
	this->follow_distance_zero_speed = distanceToTarget;
	return;
}


float smoothstep(float edge0, float edge1, float x) {
	// Scale, bias and saturate x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	// Evaluate polynomial
	return x * x * (3 - 2 * x);
}

float smootherstep(float edge0, float edge1, float x) {
	// Scale, and clamp x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	// Evaluate polynomial
	return x * x * x * (x * (x * 6 - 15) + 10);
}

float clamp(float x, float lowerlimit, float upperlimit) {
	if (x < lowerlimit)
		x = lowerlimit;
	if (x > upperlimit)
		x = upperlimit;
	return x;
}

void Camera::m_UpdateFollowCamera_GOOD(double deltaTime)
{
	// Using the smoothstep from Wikipedia:
	// https://en.wikipedia.org/wiki/Smoothstep

	// Calculate ideal location in world
	glm::vec3 camIdealPositionWorld
		= this->target + this->follow_idealCameraLocationRelToTarget;

	// How far away from the object are we?
	glm::vec3 vectorToTarget = camIdealPositionWorld - this->eye;
	glm::vec3 vectorToTargetNormal = glm::normalize(vectorToTarget);

	// Pro tip: don't use .lenth();
	float currentDistanceToTarget = glm::length(vectorToTarget);


	// Pass all that into the smooth (or smoother) step 
	// 1st is distance to Zero Speed "circle"
	// 2nd is distance to Max Speed "circle"
	float speedRatio
		= smootherstep(this->follow_distance_zero_speed,
			this->follow_distance_max_speed,
			currentDistanceToTarget);

	float scaledMaxSpeed = speedRatio * this->follow_distance_max_speed;

	// Ideal max speed
	glm::vec3 vecMaxSpeed = glm::vec3(scaledMaxSpeed,
		scaledMaxSpeed,
		scaledMaxSpeed);

	this->velocity = vecMaxSpeed * vectorToTargetNormal;

	return;
}


void Camera::m_UpdateFollowCamera_SUCKS(double deltaTime)
{
	// Calculate ideal location in world
	glm::vec3 camIdealPositionWorld
		= this->target + this->follow_idealCameraLocationRelToTarget;

	// How far away from the object are we?
	glm::vec3 vectorToTarget = camIdealPositionWorld - this->eye;

	// Normalized vector (length = 1)
	// Giving DIRECTION, then.
	glm::vec3 vectorToTargetNormal = glm::normalize(vectorToTarget);

	// Distance to target? 
	// Pro tip: don't use .lenth();
	float currentDistanceToTarget = glm::length(vectorToTarget);


	// Quick-n-Dirty version
	if (currentDistanceToTarget >= this->follow_distance_max_speed)
	{	// Set the velocity to full
		// Scale this "max speed" by the direction vector
		// (normalized camera to target vector)
		// Calculate maximum speed (in a vec3)
		glm::vec3 vecMaxSpeed = glm::vec3(
			this->follow_distance_max_speed,
			this->follow_distance_max_speed,
			this->follow_distance_max_speed);
		this->velocity = vecMaxSpeed * vectorToTargetNormal;
	}
	else if (currentDistanceToTarget <= this->follow_distance_max_speed)
	{	// The "slow down" area.
		// Use the smoothstep to determine velocity
		// Scale this area to a range from 0 to 1 (smooth step)
		float slowDownRangeLength =
			this->follow_distance_max_speed - this->follow_distance_zero_speed;
		// Distance to the zero length value
		float distanceToZeroCircle =
			currentDistanceToTarget - this->follow_distance_zero_speed;
		// Get the ratio:
		float distanceInSlowDownZoneZeroToOne =
			distanceToZeroCircle / slowDownRangeLength;

		// Use that ratio to adjust the velocity
		// Use smoothstep to calcualte this
		float distanceWithSmoothStep = smoothstep(0, 1, distanceInSlowDownZoneZeroToOne);

		float scaledVelocity =
			distanceWithSmoothStep * this->follow_max_speed;

		glm::vec3 scaledVel = glm::vec3(scaledVelocity, scaledVelocity, scaledVelocity);

		this->velocity = scaledVel * vectorToTargetNormal;
	}
	else// (currentDistanceToTarget <= this->follow_distance_min_speed)
	{
		// Stop
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}


	return;
}
