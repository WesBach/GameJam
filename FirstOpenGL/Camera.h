#ifndef _cCamera_HG_
#define _cCamera_HG_

#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class Camera
{
public:
	Camera();

	glm::vec3 eye;			// position
	glm::vec3 target;
	glm::vec3 up;
	glm::vec3 velocity;		// For "fly camera", like in blender
	glm::vec3 accel;		// For "thruster" like with rockets
							// For following, etc. 
	void updateTick(double deltaTime);

	enum eMode
	{
		MANUAL,			// Move along the axes (lookat)
		FOLLOW_CAMERA,	// Follows a target (lookat)
		FLY_CAMERA_USING_LOOK_AT,	// Here, you use the "target" as direction
									// you want to go. This allows you to transition
									// from the FOLLOW_CAMERA to FLY seamlessly
		FLY_CAMERA_GARBAGE_DONT_USE	// Movement based on direction of gaze
									// Use quaternion orientation
									// "catch"  is no LOOKAT
	};

	void setCameraMode(eMode cameraMode);
	eMode cameraMode;

	// Follow camera
	void Follow_SetOrUpdateTarget(glm::vec3 target);
	void Follow_SetIdealCameraLocation(glm::vec3 relativeToTarget);
	void Follow_SetMaxFollowSpeed(float speed);
	void Follow_SetDistanceMaxSpeed(float distanceToTarget);
	void Follow_SetDistanceMinSpeed(float distanceToTarget);

	glm::vec3 follow_idealCameraLocationRelToTarget;
	float follow_max_speed;
	float follow_distance_max_speed;
	float follow_distance_zero_speed;

	// ************************************************************
	// For the "fly camera":
	// +ve is along z-axis
	void Fly_moveForward(float distanceAlongRelativeZAxis_PosIsForward);
	void Fly_moveRightLeft(float distanceAlongRelativeXAxis_PosIsRight);
	void Fly_moveUpDown(float distanceAlongRelativeYAxis_PosIsUp);
	void Fly_move(glm::vec3 directionIWantToMove_Zforward_Yup_Xleftright);
	// +ve is right
	void Fly_turn(float turnDegreesPosIsRight);
	void Fly_turn_RightLeft(float turnDegreesPosIsRight);
	// +ve it up
	void Fly_pitch(float pitchDegreesPosIsNoseUp);
	void Fly_pitch_UpDown(float pitchDegreesPosIsNoseUp);
	// +ve is Clock-wise rotation (from nose to tail)
	void Fly_yaw(float pitchDegreesPosIsClockWise);
	void Fly_yaw_CWorCCW(float pitchDegreesPosIsClockWise);
	// 
	// You can use this to change the target from wherever it is
	//	to, say, 1.0 units from the front of the camera. 
	void RelocateTargetInFrontOfCamera(float howFarAwayFromFront);

	// ************************************************************

	void overwrtiteQOrientationFormEuler(glm::vec3 eulerAxisOrientation);
	// NOTE: Use THIS, not just setting the values
	void adjustQOrientationFormDeltaEuler(glm::vec3 eulerAxisOrientChange);

	//	glm::mat4 getMat4FromOrientation(void);
	glm::mat4 getViewMatrix(void);


	// 
	glm::quat qOrientation;

	glm::vec3 EulerAngles;	// Ya get gimbal lock, yo.
private:
	// Used for the directional calculations to move the camera
	glm::vec3 m_calcDirectionVectorFromTarget(void);
	void m_UpdateFollowCamera_SUCKS(double deltaTime);
	void m_UpdateFollowCamera_GOOD(double deltaTime);
};

#endif
