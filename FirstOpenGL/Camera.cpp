#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


Camera::Camera()
{
	// Default OpenGL 1.1 was on z axis looking at the origin
	this->eye = glm::vec3(0.0f, 0.0f, 1.0f);
	this->target = glm::vec3(0.0f, 0.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Set quaternion to some default rotation value
	this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	this->accel = glm::vec3(0.0f);
	this->velocity = glm::vec3(0.0f);

	return;
}


// For following, etc. 
void Camera::updateTick(double deltaTime)
{

	if (this->cameraMode == Camera::FOLLOW_CAMERA)
	{	// update the velocity of the camera 
		this->m_UpdateFollowCamera_GOOD(deltaTime);

		// Explicit forward Euler, like in the physics loop
		// Acceleration comes form velocity
		glm::vec3 accelThisStep = this->accel * static_cast<float>(deltaTime);
		this->velocity += accelThisStep;

		// Position comes from velocity over time
		glm::vec3 velChangeThisStep = this->velocity * static_cast<float>(deltaTime);
		this->eye += velChangeThisStep;
	}


	return;
}

void Camera::setCameraMode(eMode cameraMode)
{
	// Yes, it's an enum, but you can pass anything, so double-check
	switch (cameraMode)
	{
	case Camera::eMode::FLY_CAMERA_GARBAGE_DONT_USE:
		cameraMode = Camera::FLY_CAMERA_USING_LOOK_AT;
		assert("What are you doing with your life? DON'T use the garbage camera");
		break;
	case Camera::eMode::FLY_CAMERA_USING_LOOK_AT:
	case Camera::eMode::FOLLOW_CAMERA:
	case Camera::eMode::MANUAL:
		this->cameraMode = cameraMode;
		return;
	}

	// Something is wrong, so set to manual
	this->cameraMode = Camera::eMode::MANUAL;
	return;
}
glm::mat4 Camera::getViewMatrix(void)
{
	// Based on the mode, calculate the view matrix
	switch (cameraMode)
	{
	case Camera::eMode::MANUAL:
		// Use LookAT
		break;
	case Camera::eMode::FOLLOW_CAMERA:
	{
		glm::mat4 matView = glm::lookAt(this->eye,
			this->target,
			glm::vec3(0.0f, 1.0f, 0.0f)); // UP
		return matView;
	}
	break;
	case Camera::eMode::FLY_CAMERA_USING_LOOK_AT:
	{
		glm::mat4 matView = glm::lookAt(this->eye,
			this->target,
			glm::vec3(0.0f, 1.0f, 0.0f)); // UP
		return matView;
	}
	break;

	// ************************************************************************
	// This quaternion based mess is, well, a mess. Don't use as is
	case Camera::eMode::FLY_CAMERA_GARBAGE_DONT_USE:
		// Use same process as with drawing an object:
		// Combine transform with rotation, and return that

		glm::mat4 matCamTrans = glm::mat4x4(1.0f);
		matCamTrans = glm::translate(matCamTrans, this->eye);
		//		matCamView = matCamView * matCamTrans;

		// Like many things in GML, the conversion is done in the constructor
		glm::mat4 matCamRotate = glm::mat4(this->qOrientation);

		glm::mat4 matCamView = matCamTrans * matCamRotate;

		return matCamView;
		break;
		// ************************************************************************
	}
	// You need to check what you are doign with your life!
	// return the identity matrix
	return glm::mat4(1.0f);
}

//matView = glm::lookAt( g_cameraXYZ,						// "eye" or "camera" position
//					   g_cameraTarget_XYZ,		// "At" or "target" 
//					   glm::vec3( 0.0f, 1.0f, 0.0f ) );	// "up" vector



//// ************************************************************
//
//void Camera::setOrientationFromEuler(glm::vec3 eulerAngles);
//glm::mat4 Camera::getMat4FromOrientation(void);

void Camera::overwrtiteQOrientationFormEuler(glm::vec3 eulerAxisOrientation)
{
	// Calcualte the quaternion represnetaiton of this Euler axis
	// NOTE: We are OVERWRITING this..
	this->qOrientation = glm::quat(eulerAxisOrientation);

	return;
}

void Camera::adjustQOrientationFormDeltaEuler(glm::vec3 eulerAxisOrientChange)
{
	// How do we combine two matrices?
	// That's also how we combine quaternions...

	// So we want to "add" this change in oriention
	glm::quat qRotationChange = glm::quat(eulerAxisOrientChange);

	// Mulitply it by the current orientation;
	this->qOrientation = this->qOrientation * qRotationChange;

	return;
}
