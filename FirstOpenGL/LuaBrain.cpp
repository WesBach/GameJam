#include "LuaBrain.h"
#include "CommandManager.h"

extern CommandManager* theParallelCommands;
extern CommandManager* theSerialCommands;

LuaBrain::LuaBrain() {
	luaState = luaL_newstate();
	//open the libs
	luaL_openlibs(luaState);

	/* register our function */
	lua_pushcfunction(this->luaState, moveToDistanceVelocity);
	lua_setglobal(this->luaState, "moveToDistanceVelocity");

	lua_pushcfunction(this->luaState, moveToDistanceTime);
	lua_setglobal(this->luaState, "moveToDistanceTime");

	lua_pushcfunction(this->luaState, moveToDistanceTime);
	lua_setglobal(this->luaState, "moveToDistanceTime");

	lua_pushcfunction(this->luaState, orientToTime);
	lua_setglobal(this->luaState, "orientToTime");

	lua_pushcfunction(this->luaState, orientToTimeWorldSpace);
	lua_setglobal(this->luaState, "orientToTimeWorldSpace");

	lua_pushcfunction(this->luaState, getNumObjectsParallel);
	lua_setglobal(this->luaState, "getNumObjectsParallel");

	lua_pushcfunction(this->luaState, getNumObjectsSerial);
	lua_setglobal(this->luaState, "getNumObjectsSerial");

	lua_pushcfunction(this->luaState, curveXY);
	lua_setglobal(this->luaState, "curveXY");

	lua_pushcfunction(this->luaState, curveYXZ);
	lua_setglobal(this->luaState, "curveYXZ");

	lua_pushcfunction(this->luaState, curveYZX);
	lua_setglobal(this->luaState, "curveYZX");
}

LuaBrain::~LuaBrain() {

}

int LuaBrain::moveToDistanceVelocity(lua_State* theState) {
	//glm::vec3 pointToMoveTo, cGameObject* theObjectToMove, glm::vec3 velocity
	//get the items from the lua script
	int n = lua_gettop(theState);
	bool isInGroup = false;
	bool isParallel = false;
	float xPoint = 0.0f;
	float yPoint = 0.0f;
	float zPoint = 0.0f;
	int ObjectIndex;
	float xVel = 0.0f;
	float yVel = 0.0f;
	float zVel = 0.0f;

	if (n == 7)
	{
		isInGroup = lua_toboolean(theState, 1);
		isParallel = lua_toboolean(theState, 2);
		xPoint = lua_tonumber(theState, 3);
		yPoint = lua_tonumber(theState, 4);
		zPoint = lua_tonumber(theState, 5);
		ObjectIndex = lua_tonumber(theState, 6);
		xVel = lua_tonumber(theState, 7);
		yVel = lua_tonumber(theState, 8);
		zVel = lua_tonumber(theState, 9);

		if (isInGroup && isParallel)
		{
			theParallelCommands->createMoveToDistanceVelocity(xPoint, yPoint, zPoint, ObjectIndex, xVel, yVel, zVel);
		}
		else if (isInGroup && !isParallel)
		{
			theSerialCommands->createMoveToDistanceVelocity(xPoint, yPoint, zPoint, ObjectIndex, xVel,yVel,zVel);
		}

		lua_pushnumber(theState, 0);

		return 0;
	}
	return 1;
}

int LuaBrain::moveToDistanceTime(lua_State* theState) {
	//glm::vec3 pointToMoveTo, cGameObject* theObjectToMove,float timeItShouldTake
	//get the items from the lua script
	int n = lua_gettop(theState);
	bool isInGroup = false;
	bool isParallel = false;
	float xPoint = 0.0f;
	float yPoint = 0.0f;
	float zPoint = 0.0f;
	int ObjectIndex;
	float time = 0.0f;

	if (n == 7)
	{
		isInGroup = lua_toboolean(theState, 1);
		isParallel = lua_toboolean(theState, 2);
		xPoint = lua_tonumber(theState, 3);
		yPoint = lua_tonumber(theState, 4);
		zPoint = lua_tonumber(theState, 5);
		ObjectIndex = lua_tonumber(theState, 6);
		time = lua_tonumber(theState, 7);
		//TODO
		//Create the command with the correct manager based on the bools above
		if (isInGroup && isParallel)
		{
			theParallelCommands->createMoveToDistanceTime(xPoint,yPoint,zPoint,ObjectIndex,time);
		}
		else if (isInGroup && !isParallel)
		{
			theSerialCommands->createMoveToDistanceTime(xPoint, yPoint, zPoint, ObjectIndex, time);
		}

		lua_pushnumber(theState, 0);

		return 0;
	}
	return 1;
}

int LuaBrain::orientToTimeWorldSpace(lua_State* theState) {
	//get the items from the lua script
	int n = lua_gettop(theState);
	bool isInGroup = false;
	bool isParallel = false;
	float xDegrees = 0.0f;
	float yDegrees = 0.0f;
	float zDegrees = 0.0f;

	int ObjectIndex;
	float time = 0.0f;
	if (n == 7)
	{
		isInGroup = lua_toboolean(theState, 1);
		isParallel = lua_toboolean(theState, 2);
		xDegrees = lua_tonumber(theState, 3);
		yDegrees = lua_tonumber(theState, 4);
		zDegrees = lua_tonumber(theState, 5);
		ObjectIndex = lua_tonumber(theState, 6);
		time = lua_tonumber(theState, 7);

		//TODO
		//Create the command with the correct manager based on the bools above

		if (isInGroup && isParallel)
		{
			theParallelCommands->createOrientToWorldSpace(xDegrees, yDegrees, zDegrees, ObjectIndex, time);
		}
		else if (isInGroup && !isParallel)
		{
			theSerialCommands->createOrientToWorldSpace(xDegrees, yDegrees, zDegrees, ObjectIndex, time);
		}

		lua_pushnumber(theState, 0);

		return 0;
	}
	return 1;
}

int LuaBrain::orientToTime(lua_State* theState) {

	//get the items from the lua script
	int n = lua_gettop(theState);
	bool isInGroup = false;
	bool isParallel = false;
	float xDegrees = 0.0f;
	float yDegrees = 0.0f;
	float zDegrees = 0.0f;

	int ObjectIndex;
	float time = 0.0f;
	if (n == 7)
	{
		isInGroup = lua_toboolean(theState, 1);
		isParallel = lua_toboolean(theState, 2);
		xDegrees = lua_tonumber(theState, 3);
		yDegrees = lua_tonumber(theState, 4);
		zDegrees = lua_tonumber(theState, 5);
		ObjectIndex = lua_tonumber(theState, 6);
		time = lua_tonumber(theState, 7);

		//TODO
		//Create the command with the correct manager based on the bools above

		if (isInGroup && isParallel)
		{
			theParallelCommands->createOrientToTime(xDegrees,yDegrees,zDegrees,ObjectIndex,time);
		}
		else if (isInGroup && !isParallel)
		{
			theSerialCommands->createOrientToTime(xDegrees, yDegrees, zDegrees, ObjectIndex, time);
		}

		lua_pushnumber(theState, 0);

		return 0;
	}
	return 1;
}

int LuaBrain::curveXY(lua_State* theState) {
	//get the items from the lua script
	int n = lua_gettop(theState);
	bool isInGroup = false;
	bool isParallel = false;
	int ObjectIndex;

	if (n == 3)
	{
		isInGroup = lua_toboolean(theState, 1);
		isParallel = lua_toboolean(theState, 2);	
		ObjectIndex = lua_tonumber(theState, 3);

		//TODO
		//Create the command with the correct manager based on the bools above

		if (isInGroup && isParallel)
		{
			theParallelCommands->createCurveXY(ObjectIndex);
		}
		else if (isInGroup && !isParallel)
		{
			theSerialCommands->createCurveXY(ObjectIndex);
		}

		lua_pushnumber(theState, 0);

		return 0;
	}
	return 1;
}

int LuaBrain::curveYXZ(lua_State* theState) {
	//get the items from the lua script
	//cGameObject* theObject, bool negPos,float constVel, int iterations
	int n = lua_gettop(theState);
	bool isInGroup = false;
	bool isParallel = false;
	bool negPos;
	int ObjectIndex;
	float constVel;
	int lengthOfCurve;
	if (n == 6)
	{
		isInGroup = lua_toboolean(theState, 1);
		isParallel = lua_toboolean(theState, 2);
		ObjectIndex = lua_tonumber(theState, 3);
		negPos = lua_toboolean(theState, 4);
		constVel = lua_tonumber(theState, 5);
		lengthOfCurve = lua_tonumber(theState, 6);
		//TODO
		//Create the command with the correct manager based on the bools above

		if (isInGroup && isParallel)
		{
			theParallelCommands->createCurveYXZ(ObjectIndex,negPos,constVel,lengthOfCurve);
		}
		else if (isInGroup && !isParallel)
		{
			theSerialCommands->createCurveYXZ(ObjectIndex, negPos, constVel, lengthOfCurve);
		}

		lua_pushnumber(theState, 0);

		return 0;
	}
	return 1;
}

int LuaBrain::curveYZX(lua_State* theState) {
	//cGameObject* theObject, bool negPos
	//get the items from the lua script
	int n = lua_gettop(theState);
	bool isInGroup = false;
	bool isParallel = false;
	bool negPos;
	int ObjectIndex;

	if (n == 4)
	{
		isInGroup = lua_toboolean(theState, 1);
		isParallel = lua_toboolean(theState, 2);
		ObjectIndex = lua_tonumber(theState, 3);
		negPos = lua_toboolean(theState, 4);

		//TODO
		//Create the command with the correct manager based on the bools above

		if (isInGroup && isParallel)
		{
			theParallelCommands->createCurveYZX(ObjectIndex, negPos);
		}
		else if (isInGroup && !isParallel)
		{
			theSerialCommands->createCurveYZX(ObjectIndex, negPos);
		}

		lua_pushnumber(theState, 0);

		return 0;
	}
	return 1;
}

int LuaBrain::followObject(lua_State* theState) {
	return 0;
}

int LuaBrain::getNumObjectsParallel(lua_State* theState) {
	int n = lua_gettop(theState);
	int numObjects = 0;

	if (n == 1)
	{
		numObjects = lua_tonumber(theState, 1);
		theParallelCommands->numCommands = numObjects;
	}
	return 0;
}

int LuaBrain::getNumObjectsSerial(lua_State* theState) {
	int n = lua_gettop(theState);
	int numObjects = 0;

	if (n == 1)
	{
		numObjects = lua_tonumber(theState, 1);
		theSerialCommands->numCommands = numObjects;

		lua_pushnumber(theState, 0);
	}

	return 0;
}

int LuaBrain::locationBasedTrigger(lua_State* theState) {
	return 0;
}


void LuaBrain::runMoveToDT() {
	luaL_dofile(this->luaState, "../luaScripts/moveToDistanceTime.lua");
}

void LuaBrain::runMoveToDV() {
	luaL_dofile(this->luaState, "../luaScripts/moveToDistanceVelocity.lua");
}

void LuaBrain::runOrientToTime() {
	luaL_dofile(this->luaState, "../luaScripts/orientToTime.lua");
}

void LuaBrain::runOrientToTimeWorldPosition() {
	luaL_dofile(this->luaState, "../luaScripts/orientToTimeWorldPosition.lua");
}


  //////////////////////////
 //	IMPORTANT LUA CALLS  //
//////////////////////////

void LuaBrain::runParallel() {
	luaL_dofile(this->luaState, "../luaScripts/parallel.lua");
}

void LuaBrain::runSerial() {
	luaL_dofile(this->luaState, "../luaScripts/serial.lua");
}

void LuaBrain::runBoth() {
	luaL_dofile(this->luaState, "../luaScripts/both.lua");
}