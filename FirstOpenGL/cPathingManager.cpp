#include "cPathingManager.h"
#include "PathingUtilities.h"
#include "Utilities.h"
#include "cGameObject.h"

cPathingManager::cPathingManager() {

}

cPathingManager::~cPathingManager() {

}

//Name:		getShortestPath
//Purpose:	Calls the get shortest path function from the PathingUtilites and passes the appropriate info.
//Return:	bool
bool cPathingManager::getShortestPath() {
	findShortestPathAStar(this->startingIndex, this->endingIndex, this->theShortestPath);

	if (theShortestPath.size() > 0)
		return true;

	return false;
}

//Name:		readMazeFromFile
//Purpose:	Reads in the node state and values from the passed in file. 
//Return:	bool
bool cPathingManager::readMazeFromFile(std::string path, std::vector<cGameObject*>& theObjects) {

	std::vector<bool> vecIsActiveNode;
	std::vector<float> vecPathingValues;
	std::string tokenToFind = "";
	float numNodes = 0.0f;
	float tempFloat = 0.0f;
	bool tempIsActive = false;
	int startNodeIndex = 0;
	int endNodeIndex = 0;

	//open the file for reading
	std::ifstream file(path.c_str());

	//if the file didn't open return false
	if (!file.is_open())
		return false;

	//get the number of nodes
	tokenToFind = "NUM_NODES";
	ReadFileToToken(file, tokenToFind);
	file >> numNodes;

	//read in all the isActive values
	tokenToFind = "MAZE_DESCRIPTION";
	ReadFileToToken(file, tokenToFind);
	for (int i = 0; i < numNodes; i++)
	{
		file >> tempIsActive;
		vecIsActiveNode.push_back(tempIsActive);
	}

	//reade in the index of the starting node
	tokenToFind = "STARTING_NODE";
	ReadFileToToken(file, tokenToFind);
	file >> startNodeIndex;

	//read in the index of the end node
	tokenToFind = "FINISH_NODE";
	ReadFileToToken(file, tokenToFind);
	file >> endNodeIndex;

	//populate the start and end index
	this->endingIndex = endNodeIndex;
	this->startingIndex = startNodeIndex;

	//read in all the values for the nodes
	tokenToFind = "PATHING_VALUES";
	ReadFileToToken(file, tokenToFind);
	for (int nodeValueIndex = 0; nodeValueIndex < numNodes; nodeValueIndex++)
	{
		file >> tempFloat;
		vecPathingValues.push_back(tempFloat);
	}

	//add all the info the game objects
	for (int objectIndex = 0; objectIndex < numNodes; objectIndex++)
	{
		theObjects[objectIndex]->pathingNode->isActive = vecIsActiveNode[objectIndex];
		theObjects[objectIndex]->pathingNode->value = vecPathingValues[objectIndex];
		//if the node is not travelable set the texture to rust
		if (theObjects[objectIndex]->pathingNode->isActive == false)
		{
			theObjects[objectIndex]->vecMehs2DTextures.clear();
			theObjects[objectIndex]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("lava.bmp", 1.0f));
			theObjects[objectIndex]->vecMehs2DTextures.push_back(sTextureBindBlendInfo("lava.bmp", 1.0f));
		}
		else
		{
			theObjects[objectIndex]->bHasAABB = false;
		}
	}
}
