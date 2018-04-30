#ifndef _CommandManager_HG_
#define _CommandManager_HG_

#include <vector>
class iCommand;

class CommandManager {
public:
	CommandManager(bool parallel);
	~CommandManager();

	void createOrientToTime(float xDegrees, float yDegrees, float zDegrees, int index, float time);
	void createOrientToWorldSpace(float xDegrees, float yDegrees, float zDegrees, int index, float time);
	//glm::vec3 pointToMoveTo, cGameObject* theObjectToMove, glm::vec3 velocity
	void createMoveToDistanceVelocity(float xPoint,float yPoint,float zPoint,int index,float xVel,float yVel,float zVel);
	void createMoveToDistanceTime(float xPoint, float yPoint, float zPoint, int index, float time);

	//createCurveXY(ObjectIndex)
	//createCurveYXZ(ObjectIndex,negPos,constVel,lengthOfCurve)
	//createCurveYZX(ObjectIndex, negPos)

	void createCurveXY(int index);
	void createCurveYXZ(int index, bool negPos, float constVel, int intervals);
	void createCurveYZX(int index, bool negPos);

	void updateCommands(double deltaTime);
	int getNumCommandsInVector();
	int numCommands;

private:
	bool isParallel;

	void updateParallel(double deltaTime);
	void updateSerial(double deltaTime);
	std::vector<iCommand*> theCommands;
};
#endif // !_CommandManager_HG_
