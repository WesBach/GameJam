#ifndef _cPathingManager_HG_
#define _cPAthingManager_HG_
#include <vector>
#include "cPathingNode.h"

class cGameObject;

class cPathingManager {
public:
	cPathingManager();
	~cPathingManager();
	
	std::vector<cPathingNode*> theShortestPath;
	std::vector<cPathingNode*> theNodes;

	int startingIndex;
	int endingIndex;

	bool getShortestPath();
	bool cPathingManager::readMazeFromFile(std::string path, std::vector<cGameObject*>& theObjects);
};

#endif // !_cPathingManager_HG_
