#ifndef _LuaBrain_HG_
#define _LuaBrain_HG_
#include <lua\lua.hpp>

class LuaBrain {
public:
	LuaBrain();
	~LuaBrain();

	lua_State * luaState;
	static int moveToDistanceTime(lua_State* theState);
	static int moveToDistanceVelocity(lua_State* theState);
	static int orientToTime(lua_State* theState);
	static int orientToTimeWorldSpace(lua_State* theState);
	static int followObject(lua_State* theState);
	static int locationBasedTrigger(lua_State* theState);

	static int curveXY(lua_State* theState);
	static int curveYXZ(lua_State* theState);
	static int curveYZX(lua_State* theState);



	static int getNumObjectsParallel(lua_State* theState);
	static int getNumObjectsSerial(lua_State* theState);

	void runParallel();
	void runSerial();
	void runBoth();

	void runMoveToDT();
	void runMoveToDV();
	void runOrientToTime();
	void runOrientToTimeWorldPosition();
};
#endif // !_LuaBrain_HG_
