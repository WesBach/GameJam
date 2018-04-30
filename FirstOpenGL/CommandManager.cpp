#include "CommandManager.h"
#include "cGameObject.h"
//commands
#include "OrientToTime.h"
#include "OrientToWorldSpace.h"
#include "MoveToDistanceTime.h"
#include "MoveToDitanceVelocity.h"
#include "CurveXY.h"
#include "CurveYXZ.h"
#include "CurveYZX.h"

extern std::vector<cGameObject*> g_vecGameObjects;

CommandManager::CommandManager(bool parallel)
{
	this->isParallel = parallel;
	this->numCommands = -1;
}

CommandManager::~CommandManager() {

}

void CommandManager::createOrientToTime(float xDegrees, float yDegrees, float zDegrees, int index, float time) {
	OrientToTime* tempOrient = new OrientToTime(glm::vec3(xDegrees, yDegrees, zDegrees), g_vecGameObjects[index], time);
	this->theCommands.push_back(tempOrient);
}

void CommandManager::createOrientToWorldSpace(float xDegrees, float yDegrees, float zDegrees, int index, float time) {
	OrientToWorldSpace* tempOrient = new OrientToWorldSpace(glm::vec3(xDegrees,yDegrees,zDegrees), g_vecGameObjects[index], time);
	this->theCommands.push_back(tempOrient);
}
//glm::vec3 pointToMoveTo, cGameObject* theObjectToMove, glm::vec3 velocity
void CommandManager::createMoveToDistanceVelocity(float xPoint, float yPoint, float zPoint, int index, float xVel, float yVel, float zVel) {
	MoveToDistanceVelocity* tempMoveTo = new MoveToDistanceVelocity(glm::vec3(xPoint,yPoint,zPoint),g_vecGameObjects[index],glm::vec3(xVel,yVel,zVel));
	this->theCommands.push_back(tempMoveTo);
}

void CommandManager::createMoveToDistanceTime(float xPoint, float yPoint, float zPoint, int index, float time) {
	MoveToDistanceTime* tempMoveTo = new MoveToDistanceTime(glm::vec3(xPoint, yPoint, zPoint), g_vecGameObjects[index], time);
	this->theCommands.push_back(tempMoveTo);
}

void CommandManager::createCurveXY(int index) {
	CurveXY* theCurve = new CurveXY(g_vecGameObjects[index]);
	this->theCommands.push_back(theCurve);
}

void CommandManager::createCurveYXZ(int index, bool negPos, float constVel, int intervals) {
	CurveYXZ* theCurve = new CurveYXZ(g_vecGameObjects[index],negPos,constVel,intervals);
	this->theCommands.push_back(theCurve);
}

void CommandManager::createCurveYZX(int index, bool negPos) {
	CurveYZX* theCurve = new CurveYZX(g_vecGameObjects[index], negPos);
	this->theCommands.push_back(theCurve);
}



void CommandManager::updateCommands(double deltaTime) {
	if (this->isParallel)
	{
		updateParallel(deltaTime);
	}
	else
		this->updateSerial(deltaTime);
}

void CommandManager::updateParallel(double deltaTime) {
	for (int i = 0; i < this->theCommands.size(); i++)
	{
		if (this->theCommands[i]->getDone() == true)
			continue;

		this->theCommands[i]->update(deltaTime);
	}
}

void CommandManager::updateSerial(double deltaTime) {
	for (int i = 0; i < this->theCommands.size(); i++)
	{
		//if the current command isn't finished break, otherwise go to the next one.
		if (this->theCommands[i]->getDone() == false)
		{
			this->theCommands[i]->update(deltaTime);
			break;
		}
		else
			continue;
	}
}

int CommandManager::getNumCommandsInVector() {
	return this->theCommands.size();
}
