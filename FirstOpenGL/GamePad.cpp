#include "GamePad.h"
#include "globalOpenGL_GLFW.h"
#include "cGameObject.h"
#include <glm\vec3.hpp>
#include <iostream>
#include "cPlayer.h"
#include "cSceneManager.h"

extern glm::vec3 g_cameraXYZ;
extern glm::vec3 g_cameraTarget_XYZ;
extern glm::vec3 g_cameraBackupPosition;

extern sScene* g_pCurrentScene;
extern double deltaTime;
extern GLFWwindow*  g_pGLFWWindow;

GamePad::GamePad() {
	this->buttonsAndAxesPopulated = false;
}


void GamePad::resolveJoystickInteractions() {
	//do the axes updates
	if (axesCount > 0) {
		for (int i = 0; i < this->axesCount; i++) {
			handleAxesUpdate(i);
		}
	}

	//do the button updates 
	if (buttonCount > 0) {
		for (int i = 0; i < this->buttonCount; i++) {
			handleButtonUpdate(i);
		}
	}
}

void GamePad::getIsConnected() {
	this->isPresent = glfwJoystickPresent(GLFW_JOYSTICK_1);
}

void GamePad::getAxesArray() {
	//make sure to only do this once
	this->axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
	buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
}

void GamePad::handleAxesUpdate(int index) {
	glm::vec3 cameraPos = g_cameraXYZ;
	glm::vec3 forward = cameraPos - g_pCurrentScene->currentPlayer->thePlayerObject->position;
	forward = glm::normalize(forward);
	forward.y = 0.0f;
	glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));

	switch (index) {
	case 0:
		if (this->axes[index] > 0.1f) {
			g_pCurrentScene->currentPlayer->move(-right);
		}
		else if (this->axes[index] < -0.1f) {
			g_pCurrentScene->currentPlayer->move(right);
		}
		break;

	case 1:
		if (this->axes[index] > 0.1f) {
			g_pCurrentScene->currentPlayer->move(-forward);
			g_cameraXYZ.z += (-forward.z * g_pCurrentScene->currentPlayer->playerSpeed) * (float)1.f / 60.f;;
		}
		else if (this->axes[index] < -0.1f) {
			g_pCurrentScene->currentPlayer->move(forward);
			g_cameraXYZ.z += (forward.z * g_pCurrentScene->currentPlayer->playerSpeed) * (float)1.f / 60.f;;
		}
		break;
		
	case 2:
		if (this->axes[index] > 0.1f) {
			g_cameraXYZ.z += (-1.0f * 24.f) * (float)1.f / 60.f;
		}
		else if (this->axes[index] < -0.1f) {
			g_cameraXYZ.z += (1.0f * 24.f) * (float)1.f / 60.f;
		}
		break;

	case 3:
		if (this->axes[index] > 0.1f) {
			g_cameraXYZ.y += (1.0f * 24.f) * (float)1.f / 60.f;
		}
		else if (this->axes[index] < -0.1f) {
			g_cameraXYZ.y -= (1.0 * 24.f) * (float)1.f / 60.f;

			if (g_cameraXYZ.y < 1.f) {
				g_cameraXYZ.y = 1.f;
			}
		}
		break;

	case 4:
		if (this->axes[index] > -1.f) {
			g_pCurrentScene->currentPlayer->rotatePlayer(glm::vec3(0.f, 1.f, 0.f));
		}
		break;

	case 5:
		if (this->axes[index] > -1.f) {
			g_pCurrentScene->currentPlayer->rotatePlayer(glm::vec3(0.f, -1.f, 0.f));
		}
		break;
	}
}

void GamePad::handleButtonUpdate(int index) {
	glm::vec3 cameraPos = g_cameraXYZ;
	glm::vec3 forward = cameraPos - g_pCurrentScene->currentPlayer->thePlayerObject->position;
	forward = glm::normalize(forward);
	forward.y = 0.0f;
	glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));

	switch (index) {
	case 0: //A
		if (this->buttons[index] == GLFW_PRESS) {
			shoot = true;
		}
		else {
			shoot = false;
		}
		break;
	case 1: // B
		if (this->buttons[index] == GLFW_PRESS) {
		}
		else {

		}
		break;
	case 2: //X
		if (this->buttons[index] == GLFW_PRESS) {
		}
		else {

		}
		break;
	case 3: //Y
		if (this->buttons[index] == GLFW_PRESS) {
		}
		else {

		}
		break;
	case 4: //LeftBumper
		if (this->buttons[index] == GLFW_PRESS) {

		}
		else {

		}
		break;
	case 5: //Right Bumper
		if (this->buttons[index] == GLFW_PRESS) {
		}
		else {

		}
		break;
	case 6:	//SELECT
		if (this->buttons[index] == GLFW_PRESS) {
		}
		else {

		}
		break;
	case 7:	//START
		if (this->buttons[index] == GLFW_PRESS) {
			glfwSetWindowShouldClose(g_pGLFWWindow, GLFW_TRUE);
		}
		else {

		}
		break;
	}
}

void GamePad::updateShoot() {
	if (shoot) {
		g_pCurrentScene->currentPlayer->attack(g_pCurrentScene->currentPlayer->playerForward,deltaTime);
	}
}