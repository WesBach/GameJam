#include "Camera.h"

glm::vec3 Camera::m_calcDirectionVectorFromTarget(void)
{
	// Get a vector from the target to the eye
	// (i.e. what direction the camera is looking at)
	glm::vec3 vecDirectionAndDistanceTarget = this->target - this->eye;

	// Scale this to 1.0 (normalize)
	glm::vec3 vecDirection = glm::normalize(vecDirectionAndDistanceTarget);

	return vecDirection;
}



void Camera::Fly_moveForward(float distanceAlongRelativeZAxis_PosIsForward)
{
	// Calcualte axis components based on orientation
	glm::vec3 vecDirection = this->m_calcDirectionVectorFromTarget();

	// Scale the direction we want to go towards the direction the camera is looking at
	glm::vec3 finalDeltaPosition = vecDirection * distanceAlongRelativeZAxis_PosIsForward;

	// Update the position
	this->eye += finalDeltaPosition;

	return;
}

void Camera::Fly_moveRightLeft(float distanceAlongRelativeXAxis_PosIsRight)
{
	// Calcualte axis components based on orientation
	glm::vec3 vecDirection = this->m_calcDirectionVectorFromTarget();

	// Scale the direction we want to go towards the direction the camera is looking at
	glm::vec3 moveDelta = glm::vec3(distanceAlongRelativeXAxis_PosIsRight, 0.0f, 0.0f);
	glm::vec3 finalDeltaPosition = vecDirection * moveDelta;

	// Update the position
	this->eye += finalDeltaPosition;

	return;
}

void Camera::Fly_moveUpDown(float distanceAlongRelativeYAxis_PosIsUp)
{
	//TODO:
	return;
}
// +ve is right
void Camera::Fly_turn(float turnDegreesPosIsRight)
{
	//TODO:
	return;
}
void Camera::Fly_turn_RightLeft(float turnDegreesPosIsRight)
{
	//TODO:
	return;
}
// +ve it up
void Camera::Fly_pitch(float pitchDegreesPosIsNoseUp)
{
	//TODO:
	return;
}
void Camera::Fly_pitch_UpDown(float pitchDegreesPosIsNoseUp)
{
	//TODO:
	return;
}
// +ve is Clock-wise rotation (from nose to tail)
void Camera::Fly_yaw(float pitchDegreesPosIsClockWise)
{
	//TODO:
	return;
}
void Camera::Fly_yaw_CWorCCW(float pitchDegreesPosIsClockWise)
{
	//TODO:
	return;
}
// 
// You can use this to change the target from wherever it is
//	to, say, 1.0 units from the front of the camera. 
void Camera::RelocateTargetInFrontOfCamera(float howFarAwayFromFront)
{
	//TODO:
	return;
}