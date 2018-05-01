#include "PathingUtilities.h"
#include "cPathingManager.h"

cPathingManager* g_thePathingManager;
extern int mazeStart;
extern int mazeEnd;

//Name:		findShortestPathAStar
//Purpose:	Given the start and end index will find the "shortest" path from start to end and populate the passed in vector with the path.
//Return:	void
void findShortestPathAStar(int startingIndex, int endingIndex, std::vector<cPathingNode*>& theShortestPath) {
	//set the node connections(could be done somewhere else)
	setNodeConnections();

	cPathingNode* theStartNode = g_thePathingManager->theNodes[startingIndex];
	cPathingNode* theFinishNode = g_thePathingManager->theNodes[endingIndex];

	std::vector<cPathingNode*> closedNodes;
	std::vector<cPathingNode*> openNodes;
	//the only open node at the beginning is the starting node
	openNodes.push_back(theStartNode);
	//maps for calculations
	//constains the node and it's connected node taken (most efficient)
	std::map<unsigned int, cPathingNode*> cameFrom;
	//each nodes cost of getting from start to that node
	std::map<unsigned int, std::pair<cPathingNode*, float>> gScore;
	//each nodes cost of getting from start node to the goal going through that node
	std::map<unsigned int, std::pair<cPathingNode*, float>> fScore;
	gScore[theStartNode->nodeId] = std::pair<cPathingNode*, float>(theStartNode, theStartNode->value);
	//give start a value of 0; cost from going from start to start
	fScore[theStartNode->nodeId] = std::pair<cPathingNode*, float>(theStartNode, glm::distance(theStartNode->position, theFinishNode->position));
	float tentative_gScore;

	cPathingNode* current = new cPathingNode();

	while (openNodes.size() > 0){
		getNodeWithShortestFScoreFromOpen(openNodes, fScore, *current);

		//check based on position because there are a set amount of nodes
		if (current->nodeId == theFinishNode->nodeId){
			reconstructPath(cameFrom, current, theShortestPath, theStartNode->nodeId);
			break;
		}

		//new node to push back on closed nodes
		cPathingNode* tempNode = new cPathingNode();
		tempNode->nodeId = current->nodeId;
		tempNode->value = current->value;
		tempNode->isActive = current->isActive;
		tempNode->position = current->position;
		tempNode->theConnections = current->theConnections;

		removeNodeFromOpen(current, openNodes);
		closedNodes.push_back(tempNode);
		//add all the connections to the gScore map
		for (int i = 0; i < current->theConnections.size(); i++){
			gScore[current->theConnections[i]->nodeId].second = gScore[current->nodeId].second + glm::distance(current->position, current->theConnections[i]->position) + current->theConnections[i]->value;
			fScore[current->theConnections[i]->nodeId].first = current->theConnections[i];
			fScore[current->theConnections[i]->nodeId].second = gScore[current->theConnections[i]->nodeId].second + glm::distance(current->theConnections[i]->position, theFinishNode->position);
		}

		//go through all the node connections
		for (int i = 0; i < current->theConnections.size(); i++) {
			if (isPresent(current->theConnections[i], closedNodes)) {
				continue;		// Ignore the neighbor which is already evaluated.
			}
			else if (isPresent(current->theConnections[i], openNodes) == false && current->theConnections[i]->isActive) {
				openNodes.push_back(current->theConnections[i]);
			}
			else{//isn't an active node so continue
				continue;
			}

			// The distance from start to a neighbor
			//the "dist_between" function may vary as per the solution requirements.
			tentative_gScore = gScore[current->nodeId].second + glm::distance(current->position, current->theConnections[i]->position);

			float currentgScore = gScore[current->theConnections[i]->nodeId].second;
			if (tentative_gScore >= currentgScore && current->theConnections[i]->isActive){
				continue;		// This is not a better path.
			}

			// This path is the best until now. Record it!
			cameFrom[current->theConnections[i]->nodeId] = new cPathingNode();
			cameFrom[current->theConnections[i]->nodeId]->nodeId = current->nodeId;
			cameFrom[current->theConnections[i]->nodeId]->value = current->value;
			cameFrom[current->theConnections[i]->nodeId]->isActive = current->isActive;
			cameFrom[current->theConnections[i]->nodeId]->position = current->position;
			cameFrom[current->theConnections[i]->nodeId]->theConnections = current->theConnections;

			gScore[current->theConnections[i]->nodeId].second = tentative_gScore;

			//TODO:: fix this, glm::distance isn't accurate, should be a function that calculates the rough "distance" (distance being an accumulation of the values) from this node to the finish
			fScore[current->theConnections[i]->nodeId].second = gScore[current->theConnections[i]->nodeId].second + glm::distance(current->theConnections[i]->position, theFinishNode->position);
		}
	}

	delete current;
}

//Name:		getNodeWithShortestFScoreFromOpen
//Purpose:	Finds the node from the open vector with the smallest fScore.
//Return:	void
void getNodeWithShortestFScoreFromOpen(std::vector<cPathingNode*>& theOpenNodes, std::map<unsigned int, std::pair<cPathingNode*, float>>& fScores, cPathingNode& current){
	std::vector<std::pair<cPathingNode*, float>> theShortestOpen;

	//check the open nodes for the shortest fScores
	for (int i = 0; i < theOpenNodes.size(); i++){
		//go through the map and get the scores
		float score = fScores[theOpenNodes[i]->nodeId].second;
		theShortestOpen.push_back(std::pair<cPathingNode*, float>(theOpenNodes[i], score));
	}
	//sort so the first element will be the shortest fScore
	std::sort(theShortestOpen.begin(), theShortestOpen.end(), sortShortestBySecond);

	current = *fScores[theShortestOpen[0].first->nodeId].first;
}

//Name:		SortShortestBySecond
//Purpose:	A function used as the third argument in std::vector sort (Sorts Ascending).
//Return:	void
bool sortShortestBySecond(std::pair<cPathingNode*, float>& pairA, std::pair<cPathingNode*, float>& pairB){
	return pairA.second < pairB.second;
}

//Name:		isPresent
//Purpose:	Checks to see if a node is present in the passed in vector.
//Return:	void
bool isPresent(const cPathingNode* theNode, const std::vector<cPathingNode*>& theNodes){
	for (int i = 0; i < theNodes.size(); i++){
		//check the position because there will only ever be one node in that spot
		if (theNode->position == theNodes[i]->position)
			return true;
	}

	return false;
}

//Name:		setNodeConnections
//Purpose:	Hard code the node connections.
//Return:	void
void setNodeConnections() {
	{
		// 0 = 1,8,9
		g_thePathingManager->theNodes[0]->theConnections.push_back(g_thePathingManager->theNodes[8]);
		g_thePathingManager->theNodes[0]->theConnections.push_back(g_thePathingManager->theNodes[1]);
		g_thePathingManager->theNodes[0]->theConnections.push_back(g_thePathingManager->theNodes[9]);
		//1 = 0,2,8,9,10  ->
		g_thePathingManager->theNodes[1]->theConnections.push_back(g_thePathingManager->theNodes[0]);
		g_thePathingManager->theNodes[1]->theConnections.push_back(g_thePathingManager->theNodes[2]);
		g_thePathingManager->theNodes[1]->theConnections.push_back(g_thePathingManager->theNodes[8]);
		g_thePathingManager->theNodes[1]->theConnections.push_back(g_thePathingManager->theNodes[9]);
		g_thePathingManager->theNodes[1]->theConnections.push_back(g_thePathingManager->theNodes[10]);
		//2 = 1,3,9,10,11 ->
		g_thePathingManager->theNodes[2]->theConnections.push_back(g_thePathingManager->theNodes[1]);
		g_thePathingManager->theNodes[2]->theConnections.push_back(g_thePathingManager->theNodes[3]);
		g_thePathingManager->theNodes[2]->theConnections.push_back(g_thePathingManager->theNodes[9]);
		g_thePathingManager->theNodes[2]->theConnections.push_back(g_thePathingManager->theNodes[10]);
		g_thePathingManager->theNodes[2]->theConnections.push_back(g_thePathingManager->theNodes[11]);
		//3 = 2,4,10,11,12->
		g_thePathingManager->theNodes[3]->theConnections.push_back(g_thePathingManager->theNodes[2]);
		g_thePathingManager->theNodes[3]->theConnections.push_back(g_thePathingManager->theNodes[4]);
		g_thePathingManager->theNodes[3]->theConnections.push_back(g_thePathingManager->theNodes[10]);
		g_thePathingManager->theNodes[3]->theConnections.push_back(g_thePathingManager->theNodes[11]);
		g_thePathingManager->theNodes[3]->theConnections.push_back(g_thePathingManager->theNodes[12]);
		//4 = 3,5,11,12,13->
		g_thePathingManager->theNodes[4]->theConnections.push_back(g_thePathingManager->theNodes[3]);
		g_thePathingManager->theNodes[4]->theConnections.push_back(g_thePathingManager->theNodes[5]);
		g_thePathingManager->theNodes[4]->theConnections.push_back(g_thePathingManager->theNodes[11]);
		g_thePathingManager->theNodes[4]->theConnections.push_back(g_thePathingManager->theNodes[12]);
		g_thePathingManager->theNodes[4]->theConnections.push_back(g_thePathingManager->theNodes[13]);
		//5 = 4,6,12,13,14->
		g_thePathingManager->theNodes[5]->theConnections.push_back(g_thePathingManager->theNodes[4]);
		g_thePathingManager->theNodes[5]->theConnections.push_back(g_thePathingManager->theNodes[6]);
		g_thePathingManager->theNodes[5]->theConnections.push_back(g_thePathingManager->theNodes[12]);
		g_thePathingManager->theNodes[5]->theConnections.push_back(g_thePathingManager->theNodes[13]);
		g_thePathingManager->theNodes[5]->theConnections.push_back(g_thePathingManager->theNodes[14]);
		//6 = 5,7,13,14,15->
		g_thePathingManager->theNodes[6]->theConnections.push_back(g_thePathingManager->theNodes[5]);
		g_thePathingManager->theNodes[6]->theConnections.push_back(g_thePathingManager->theNodes[7]);
		g_thePathingManager->theNodes[6]->theConnections.push_back(g_thePathingManager->theNodes[13]);
		g_thePathingManager->theNodes[6]->theConnections.push_back(g_thePathingManager->theNodes[14]);
		g_thePathingManager->theNodes[6]->theConnections.push_back(g_thePathingManager->theNodes[15]);
		//7 = 6,14,15	  ->
		g_thePathingManager->theNodes[7]->theConnections.push_back(g_thePathingManager->theNodes[6]);
		g_thePathingManager->theNodes[7]->theConnections.push_back(g_thePathingManager->theNodes[14]);
		g_thePathingManager->theNodes[7]->theConnections.push_back(g_thePathingManager->theNodes[15]);
	}

	{
		//8 = 0,1,9,16,17
		g_thePathingManager->theNodes[8]->theConnections.push_back(g_thePathingManager->theNodes[0]);
		g_thePathingManager->theNodes[8]->theConnections.push_back(g_thePathingManager->theNodes[1]);
		g_thePathingManager->theNodes[8]->theConnections.push_back(g_thePathingManager->theNodes[9]);
		g_thePathingManager->theNodes[8]->theConnections.push_back(g_thePathingManager->theNodes[16]);
		g_thePathingManager->theNodes[8]->theConnections.push_back(g_thePathingManager->theNodes[17]);
		//9 = 0,1,2,8,10,1->,17,18
		g_thePathingManager->theNodes[9]->theConnections.push_back(g_thePathingManager->theNodes[0]);
		g_thePathingManager->theNodes[9]->theConnections.push_back(g_thePathingManager->theNodes[1]);
		g_thePathingManager->theNodes[9]->theConnections.push_back(g_thePathingManager->theNodes[2]);
		g_thePathingManager->theNodes[9]->theConnections.push_back(g_thePathingManager->theNodes[8]);
		g_thePathingManager->theNodes[9]->theConnections.push_back(g_thePathingManager->theNodes[10]);
		g_thePathingManager->theNodes[9]->theConnections.push_back(g_thePathingManager->theNodes[16]);
		g_thePathingManager->theNodes[9]->theConnections.push_back(g_thePathingManager->theNodes[17]);
		g_thePathingManager->theNodes[9]->theConnections.push_back(g_thePathingManager->theNodes[18]);
		//10 = 1,2,3,9,11,17,18,19
		g_thePathingManager->theNodes[10]->theConnections.push_back(g_thePathingManager->theNodes[1]);
		g_thePathingManager->theNodes[10]->theConnections.push_back(g_thePathingManager->theNodes[2]);
		g_thePathingManager->theNodes[10]->theConnections.push_back(g_thePathingManager->theNodes[3]);
		g_thePathingManager->theNodes[10]->theConnections.push_back(g_thePathingManager->theNodes[9]);
		g_thePathingManager->theNodes[10]->theConnections.push_back(g_thePathingManager->theNodes[11]);
		g_thePathingManager->theNodes[10]->theConnections.push_back(g_thePathingManager->theNodes[17]);
		g_thePathingManager->theNodes[10]->theConnections.push_back(g_thePathingManager->theNodes[18]);
		g_thePathingManager->theNodes[10]->theConnections.push_back(g_thePathingManager->theNodes[19]);
		//11 = 2,3,4,10,12,->8,19,20
		g_thePathingManager->theNodes[11]->theConnections.push_back(g_thePathingManager->theNodes[2]);
		g_thePathingManager->theNodes[11]->theConnections.push_back(g_thePathingManager->theNodes[3]);
		g_thePathingManager->theNodes[11]->theConnections.push_back(g_thePathingManager->theNodes[4]);
		g_thePathingManager->theNodes[11]->theConnections.push_back(g_thePathingManager->theNodes[10]);
		g_thePathingManager->theNodes[11]->theConnections.push_back(g_thePathingManager->theNodes[12]);
		g_thePathingManager->theNodes[11]->theConnections.push_back(g_thePathingManager->theNodes[18]);
		g_thePathingManager->theNodes[11]->theConnections.push_back(g_thePathingManager->theNodes[19]);
		g_thePathingManager->theNodes[11]->theConnections.push_back(g_thePathingManager->theNodes[20]);
		//12 = 3,4,5,11,13,->9,20,21
		g_thePathingManager->theNodes[12]->theConnections.push_back(g_thePathingManager->theNodes[3]);
		g_thePathingManager->theNodes[12]->theConnections.push_back(g_thePathingManager->theNodes[4]);
		g_thePathingManager->theNodes[12]->theConnections.push_back(g_thePathingManager->theNodes[5]);
		g_thePathingManager->theNodes[12]->theConnections.push_back(g_thePathingManager->theNodes[11]);
		g_thePathingManager->theNodes[12]->theConnections.push_back(g_thePathingManager->theNodes[13]);
		g_thePathingManager->theNodes[12]->theConnections.push_back(g_thePathingManager->theNodes[19]);
		g_thePathingManager->theNodes[12]->theConnections.push_back(g_thePathingManager->theNodes[20]);
		g_thePathingManager->theNodes[12]->theConnections.push_back(g_thePathingManager->theNodes[21]);
		//13 = 4,5,6,12,14,->0,21,22
		g_thePathingManager->theNodes[13]->theConnections.push_back(g_thePathingManager->theNodes[4]);
		g_thePathingManager->theNodes[13]->theConnections.push_back(g_thePathingManager->theNodes[5]);
		g_thePathingManager->theNodes[13]->theConnections.push_back(g_thePathingManager->theNodes[6]);
		g_thePathingManager->theNodes[13]->theConnections.push_back(g_thePathingManager->theNodes[12]);
		g_thePathingManager->theNodes[13]->theConnections.push_back(g_thePathingManager->theNodes[14]);
		g_thePathingManager->theNodes[13]->theConnections.push_back(g_thePathingManager->theNodes[20]);
		g_thePathingManager->theNodes[13]->theConnections.push_back(g_thePathingManager->theNodes[21]);
		g_thePathingManager->theNodes[13]->theConnections.push_back(g_thePathingManager->theNodes[22]);
		//14 = 5,6,7,13,15,->1,22,23
		g_thePathingManager->theNodes[14]->theConnections.push_back(g_thePathingManager->theNodes[5]);
		g_thePathingManager->theNodes[14]->theConnections.push_back(g_thePathingManager->theNodes[6]);
		g_thePathingManager->theNodes[14]->theConnections.push_back(g_thePathingManager->theNodes[7]);
		g_thePathingManager->theNodes[14]->theConnections.push_back(g_thePathingManager->theNodes[13]);
		g_thePathingManager->theNodes[14]->theConnections.push_back(g_thePathingManager->theNodes[15]);
		g_thePathingManager->theNodes[14]->theConnections.push_back(g_thePathingManager->theNodes[21]);
		g_thePathingManager->theNodes[14]->theConnections.push_back(g_thePathingManager->theNodes[22]);
		g_thePathingManager->theNodes[14]->theConnections.push_back(g_thePathingManager->theNodes[23]);
		//15 = 6,7,14,22,23->
		g_thePathingManager->theNodes[15]->theConnections.push_back(g_thePathingManager->theNodes[6]);
		g_thePathingManager->theNodes[15]->theConnections.push_back(g_thePathingManager->theNodes[7]);
		g_thePathingManager->theNodes[15]->theConnections.push_back(g_thePathingManager->theNodes[14]);
		g_thePathingManager->theNodes[15]->theConnections.push_back(g_thePathingManager->theNodes[22]);
		g_thePathingManager->theNodes[15]->theConnections.push_back(g_thePathingManager->theNodes[23]);
	}

	{
		//16 = 8,9,17,24,25
		g_thePathingManager->theNodes[16]->theConnections.push_back(g_thePathingManager->theNodes[8]);
		g_thePathingManager->theNodes[16]->theConnections.push_back(g_thePathingManager->theNodes[9]);
		g_thePathingManager->theNodes[16]->theConnections.push_back(g_thePathingManager->theNodes[17]);
		g_thePathingManager->theNodes[16]->theConnections.push_back(g_thePathingManager->theNodes[24]);
		g_thePathingManager->theNodes[16]->theConnections.push_back(g_thePathingManager->theNodes[25]);
		//17 = 8,9,10,16,18,24,25,26
		g_thePathingManager->theNodes[17]->theConnections.push_back(g_thePathingManager->theNodes[8]);
		g_thePathingManager->theNodes[17]->theConnections.push_back(g_thePathingManager->theNodes[9]);
		g_thePathingManager->theNodes[17]->theConnections.push_back(g_thePathingManager->theNodes[10]);
		g_thePathingManager->theNodes[17]->theConnections.push_back(g_thePathingManager->theNodes[16]);
		g_thePathingManager->theNodes[17]->theConnections.push_back(g_thePathingManager->theNodes[18]);
		g_thePathingManager->theNodes[17]->theConnections.push_back(g_thePathingManager->theNodes[24]);
		g_thePathingManager->theNodes[17]->theConnections.push_back(g_thePathingManager->theNodes[25]);
		g_thePathingManager->theNodes[17]->theConnections.push_back(g_thePathingManager->theNodes[26]);
		//18 = 9,10,11,17,1->,25,26,27
		g_thePathingManager->theNodes[18]->theConnections.push_back(g_thePathingManager->theNodes[9]);
		g_thePathingManager->theNodes[18]->theConnections.push_back(g_thePathingManager->theNodes[10]);
		g_thePathingManager->theNodes[18]->theConnections.push_back(g_thePathingManager->theNodes[11]);
		g_thePathingManager->theNodes[18]->theConnections.push_back(g_thePathingManager->theNodes[17]);
		g_thePathingManager->theNodes[18]->theConnections.push_back(g_thePathingManager->theNodes[19]);
		g_thePathingManager->theNodes[18]->theConnections.push_back(g_thePathingManager->theNodes[25]);
		g_thePathingManager->theNodes[18]->theConnections.push_back(g_thePathingManager->theNodes[26]);
		g_thePathingManager->theNodes[18]->theConnections.push_back(g_thePathingManager->theNodes[27]);
		//19 = 10,11,12,18,->0,26,27,28
		g_thePathingManager->theNodes[19]->theConnections.push_back(g_thePathingManager->theNodes[10]);
		g_thePathingManager->theNodes[19]->theConnections.push_back(g_thePathingManager->theNodes[11]);
		g_thePathingManager->theNodes[19]->theConnections.push_back(g_thePathingManager->theNodes[12]);
		g_thePathingManager->theNodes[19]->theConnections.push_back(g_thePathingManager->theNodes[18]);
		g_thePathingManager->theNodes[19]->theConnections.push_back(g_thePathingManager->theNodes[20]);
		g_thePathingManager->theNodes[19]->theConnections.push_back(g_thePathingManager->theNodes[26]);
		g_thePathingManager->theNodes[19]->theConnections.push_back(g_thePathingManager->theNodes[27]);
		g_thePathingManager->theNodes[19]->theConnections.push_back(g_thePathingManager->theNodes[28]);
		//20 = 11,12,13,19,->1,27,28,29,
		g_thePathingManager->theNodes[20]->theConnections.push_back(g_thePathingManager->theNodes[11]);
		g_thePathingManager->theNodes[20]->theConnections.push_back(g_thePathingManager->theNodes[12]);
		g_thePathingManager->theNodes[20]->theConnections.push_back(g_thePathingManager->theNodes[13]);
		g_thePathingManager->theNodes[20]->theConnections.push_back(g_thePathingManager->theNodes[19]);
		g_thePathingManager->theNodes[20]->theConnections.push_back(g_thePathingManager->theNodes[21]);
		g_thePathingManager->theNodes[20]->theConnections.push_back(g_thePathingManager->theNodes[27]);
		g_thePathingManager->theNodes[20]->theConnections.push_back(g_thePathingManager->theNodes[28]);
		g_thePathingManager->theNodes[20]->theConnections.push_back(g_thePathingManager->theNodes[29]);
		//21 = 12,13,14,20,->2,28,29,30
		g_thePathingManager->theNodes[21]->theConnections.push_back(g_thePathingManager->theNodes[12]);
		g_thePathingManager->theNodes[21]->theConnections.push_back(g_thePathingManager->theNodes[13]);
		g_thePathingManager->theNodes[21]->theConnections.push_back(g_thePathingManager->theNodes[14]);
		g_thePathingManager->theNodes[21]->theConnections.push_back(g_thePathingManager->theNodes[20]);
		g_thePathingManager->theNodes[21]->theConnections.push_back(g_thePathingManager->theNodes[22]);
		g_thePathingManager->theNodes[21]->theConnections.push_back(g_thePathingManager->theNodes[28]);
		g_thePathingManager->theNodes[21]->theConnections.push_back(g_thePathingManager->theNodes[29]);
		g_thePathingManager->theNodes[21]->theConnections.push_back(g_thePathingManager->theNodes[30]);
		//22 = 13,14,15,21,->3,29,30,31
		g_thePathingManager->theNodes[22]->theConnections.push_back(g_thePathingManager->theNodes[13]);
		g_thePathingManager->theNodes[22]->theConnections.push_back(g_thePathingManager->theNodes[14]);
		g_thePathingManager->theNodes[22]->theConnections.push_back(g_thePathingManager->theNodes[15]);
		g_thePathingManager->theNodes[22]->theConnections.push_back(g_thePathingManager->theNodes[21]);
		g_thePathingManager->theNodes[22]->theConnections.push_back(g_thePathingManager->theNodes[23]);
		g_thePathingManager->theNodes[22]->theConnections.push_back(g_thePathingManager->theNodes[29]);
		g_thePathingManager->theNodes[22]->theConnections.push_back(g_thePathingManager->theNodes[30]);
		g_thePathingManager->theNodes[22]->theConnections.push_back(g_thePathingManager->theNodes[31]);
		//23 = 14,15,22,30,->1
		g_thePathingManager->theNodes[23]->theConnections.push_back(g_thePathingManager->theNodes[14]);
		g_thePathingManager->theNodes[23]->theConnections.push_back(g_thePathingManager->theNodes[15]);
		g_thePathingManager->theNodes[23]->theConnections.push_back(g_thePathingManager->theNodes[22]);
		g_thePathingManager->theNodes[23]->theConnections.push_back(g_thePathingManager->theNodes[30]);
		g_thePathingManager->theNodes[23]->theConnections.push_back(g_thePathingManager->theNodes[31]);
	}

	{
		//24 = 16,17,25,32,33
		g_thePathingManager->theNodes[24]->theConnections.push_back(g_thePathingManager->theNodes[16]);
		g_thePathingManager->theNodes[24]->theConnections.push_back(g_thePathingManager->theNodes[17]);
		g_thePathingManager->theNodes[24]->theConnections.push_back(g_thePathingManager->theNodes[25]);
		g_thePathingManager->theNodes[24]->theConnections.push_back(g_thePathingManager->theNodes[32]);
		g_thePathingManager->theNodes[24]->theConnections.push_back(g_thePathingManager->theNodes[33]);
		//25 = 16,17,18,24,26,32,33,34
		g_thePathingManager->theNodes[25]->theConnections.push_back(g_thePathingManager->theNodes[16]);
		g_thePathingManager->theNodes[25]->theConnections.push_back(g_thePathingManager->theNodes[17]);
		g_thePathingManager->theNodes[25]->theConnections.push_back(g_thePathingManager->theNodes[18]);
		g_thePathingManager->theNodes[25]->theConnections.push_back(g_thePathingManager->theNodes[24]);
		g_thePathingManager->theNodes[25]->theConnections.push_back(g_thePathingManager->theNodes[26]);
		g_thePathingManager->theNodes[25]->theConnections.push_back(g_thePathingManager->theNodes[32]);
		g_thePathingManager->theNodes[25]->theConnections.push_back(g_thePathingManager->theNodes[33]);
		g_thePathingManager->theNodes[25]->theConnections.push_back(g_thePathingManager->theNodes[34]);
		//26 = 17,18,19,25,->7,33,34,35
		g_thePathingManager->theNodes[26]->theConnections.push_back(g_thePathingManager->theNodes[17]);
		g_thePathingManager->theNodes[26]->theConnections.push_back(g_thePathingManager->theNodes[18]);
		g_thePathingManager->theNodes[26]->theConnections.push_back(g_thePathingManager->theNodes[19]);
		g_thePathingManager->theNodes[26]->theConnections.push_back(g_thePathingManager->theNodes[25]);
		g_thePathingManager->theNodes[26]->theConnections.push_back(g_thePathingManager->theNodes[27]);
		g_thePathingManager->theNodes[26]->theConnections.push_back(g_thePathingManager->theNodes[33]);
		g_thePathingManager->theNodes[26]->theConnections.push_back(g_thePathingManager->theNodes[34]);
		g_thePathingManager->theNodes[26]->theConnections.push_back(g_thePathingManager->theNodes[35]);
		//27 = 18,19,20,26,->8,34,35,36
		g_thePathingManager->theNodes[27]->theConnections.push_back(g_thePathingManager->theNodes[18]);
		g_thePathingManager->theNodes[27]->theConnections.push_back(g_thePathingManager->theNodes[19]);
		g_thePathingManager->theNodes[27]->theConnections.push_back(g_thePathingManager->theNodes[20]);
		g_thePathingManager->theNodes[27]->theConnections.push_back(g_thePathingManager->theNodes[26]);
		g_thePathingManager->theNodes[27]->theConnections.push_back(g_thePathingManager->theNodes[28]);
		g_thePathingManager->theNodes[27]->theConnections.push_back(g_thePathingManager->theNodes[34]);
		g_thePathingManager->theNodes[27]->theConnections.push_back(g_thePathingManager->theNodes[35]);
		g_thePathingManager->theNodes[27]->theConnections.push_back(g_thePathingManager->theNodes[36]);
		//28 = 19,20,21,27,->9,35,36,37
		g_thePathingManager->theNodes[28]->theConnections.push_back(g_thePathingManager->theNodes[19]);
		g_thePathingManager->theNodes[28]->theConnections.push_back(g_thePathingManager->theNodes[20]);
		g_thePathingManager->theNodes[28]->theConnections.push_back(g_thePathingManager->theNodes[21]);
		g_thePathingManager->theNodes[28]->theConnections.push_back(g_thePathingManager->theNodes[27]);
		g_thePathingManager->theNodes[28]->theConnections.push_back(g_thePathingManager->theNodes[29]);
		g_thePathingManager->theNodes[28]->theConnections.push_back(g_thePathingManager->theNodes[35]);
		g_thePathingManager->theNodes[28]->theConnections.push_back(g_thePathingManager->theNodes[36]);
		g_thePathingManager->theNodes[28]->theConnections.push_back(g_thePathingManager->theNodes[37]);
		//29 = 20,21,22,28,->0,36,37,38
		g_thePathingManager->theNodes[29]->theConnections.push_back(g_thePathingManager->theNodes[20]);
		g_thePathingManager->theNodes[29]->theConnections.push_back(g_thePathingManager->theNodes[21]);
		g_thePathingManager->theNodes[29]->theConnections.push_back(g_thePathingManager->theNodes[22]);
		g_thePathingManager->theNodes[29]->theConnections.push_back(g_thePathingManager->theNodes[28]);
		g_thePathingManager->theNodes[29]->theConnections.push_back(g_thePathingManager->theNodes[30]);
		g_thePathingManager->theNodes[29]->theConnections.push_back(g_thePathingManager->theNodes[36]);
		g_thePathingManager->theNodes[29]->theConnections.push_back(g_thePathingManager->theNodes[37]);
		g_thePathingManager->theNodes[29]->theConnections.push_back(g_thePathingManager->theNodes[38]);
		//30 = 21,22,23,29,->1,37,38,39
		g_thePathingManager->theNodes[30]->theConnections.push_back(g_thePathingManager->theNodes[21]);
		g_thePathingManager->theNodes[30]->theConnections.push_back(g_thePathingManager->theNodes[22]);
		g_thePathingManager->theNodes[30]->theConnections.push_back(g_thePathingManager->theNodes[23]);
		g_thePathingManager->theNodes[30]->theConnections.push_back(g_thePathingManager->theNodes[29]);
		g_thePathingManager->theNodes[30]->theConnections.push_back(g_thePathingManager->theNodes[31]);
		g_thePathingManager->theNodes[30]->theConnections.push_back(g_thePathingManager->theNodes[37]);
		g_thePathingManager->theNodes[30]->theConnections.push_back(g_thePathingManager->theNodes[38]);
		g_thePathingManager->theNodes[30]->theConnections.push_back(g_thePathingManager->theNodes[39]);
		//31 = 22,23,30,38,->9
		g_thePathingManager->theNodes[31]->theConnections.push_back(g_thePathingManager->theNodes[22]);
		g_thePathingManager->theNodes[31]->theConnections.push_back(g_thePathingManager->theNodes[23]);
		g_thePathingManager->theNodes[31]->theConnections.push_back(g_thePathingManager->theNodes[30]);
		g_thePathingManager->theNodes[31]->theConnections.push_back(g_thePathingManager->theNodes[38]);
		g_thePathingManager->theNodes[31]->theConnections.push_back(g_thePathingManager->theNodes[39]);
	}

	{
		//32 = 24,25,33,40,41
		g_thePathingManager->theNodes[32]->theConnections.push_back(g_thePathingManager->theNodes[24]);
		g_thePathingManager->theNodes[32]->theConnections.push_back(g_thePathingManager->theNodes[25]);
		g_thePathingManager->theNodes[32]->theConnections.push_back(g_thePathingManager->theNodes[33]);
		g_thePathingManager->theNodes[32]->theConnections.push_back(g_thePathingManager->theNodes[40]);
		g_thePathingManager->theNodes[32]->theConnections.push_back(g_thePathingManager->theNodes[41]);
		//33 = 24,25,26,32,->4,40,41,42
		g_thePathingManager->theNodes[33]->theConnections.push_back(g_thePathingManager->theNodes[24]);
		g_thePathingManager->theNodes[33]->theConnections.push_back(g_thePathingManager->theNodes[25]);
		g_thePathingManager->theNodes[33]->theConnections.push_back(g_thePathingManager->theNodes[26]);
		g_thePathingManager->theNodes[33]->theConnections.push_back(g_thePathingManager->theNodes[32]);
		g_thePathingManager->theNodes[33]->theConnections.push_back(g_thePathingManager->theNodes[34]);
		g_thePathingManager->theNodes[33]->theConnections.push_back(g_thePathingManager->theNodes[40]);
		g_thePathingManager->theNodes[33]->theConnections.push_back(g_thePathingManager->theNodes[41]);
		g_thePathingManager->theNodes[33]->theConnections.push_back(g_thePathingManager->theNodes[42]);
		//34 = 25,26,27,33,->5,41,42,43
		g_thePathingManager->theNodes[34]->theConnections.push_back(g_thePathingManager->theNodes[25]);
		g_thePathingManager->theNodes[34]->theConnections.push_back(g_thePathingManager->theNodes[26]);
		g_thePathingManager->theNodes[34]->theConnections.push_back(g_thePathingManager->theNodes[27]);
		g_thePathingManager->theNodes[34]->theConnections.push_back(g_thePathingManager->theNodes[33]);
		g_thePathingManager->theNodes[34]->theConnections.push_back(g_thePathingManager->theNodes[35]);
		g_thePathingManager->theNodes[34]->theConnections.push_back(g_thePathingManager->theNodes[41]);
		g_thePathingManager->theNodes[34]->theConnections.push_back(g_thePathingManager->theNodes[42]);
		g_thePathingManager->theNodes[34]->theConnections.push_back(g_thePathingManager->theNodes[43]);
		//35 = 26,27,28,34,->6,42,43,44
		g_thePathingManager->theNodes[35]->theConnections.push_back(g_thePathingManager->theNodes[26]);
		g_thePathingManager->theNodes[35]->theConnections.push_back(g_thePathingManager->theNodes[27]);
		g_thePathingManager->theNodes[35]->theConnections.push_back(g_thePathingManager->theNodes[28]);
		g_thePathingManager->theNodes[35]->theConnections.push_back(g_thePathingManager->theNodes[34]);
		g_thePathingManager->theNodes[35]->theConnections.push_back(g_thePathingManager->theNodes[36]);
		g_thePathingManager->theNodes[35]->theConnections.push_back(g_thePathingManager->theNodes[42]);
		g_thePathingManager->theNodes[35]->theConnections.push_back(g_thePathingManager->theNodes[43]);
		g_thePathingManager->theNodes[35]->theConnections.push_back(g_thePathingManager->theNodes[44]);
		//36 = 27,28,29,35,->7,43,44,45
		g_thePathingManager->theNodes[36]->theConnections.push_back(g_thePathingManager->theNodes[27]);
		g_thePathingManager->theNodes[36]->theConnections.push_back(g_thePathingManager->theNodes[28]);
		g_thePathingManager->theNodes[36]->theConnections.push_back(g_thePathingManager->theNodes[29]);
		g_thePathingManager->theNodes[36]->theConnections.push_back(g_thePathingManager->theNodes[35]);
		g_thePathingManager->theNodes[36]->theConnections.push_back(g_thePathingManager->theNodes[37]);
		g_thePathingManager->theNodes[36]->theConnections.push_back(g_thePathingManager->theNodes[43]);
		g_thePathingManager->theNodes[36]->theConnections.push_back(g_thePathingManager->theNodes[44]);
		g_thePathingManager->theNodes[36]->theConnections.push_back(g_thePathingManager->theNodes[45]);
		//37 = 28,29,30,36,->8,44,45,46
		g_thePathingManager->theNodes[37]->theConnections.push_back(g_thePathingManager->theNodes[28]);
		g_thePathingManager->theNodes[37]->theConnections.push_back(g_thePathingManager->theNodes[29]);
		g_thePathingManager->theNodes[37]->theConnections.push_back(g_thePathingManager->theNodes[30]);
		g_thePathingManager->theNodes[37]->theConnections.push_back(g_thePathingManager->theNodes[36]);
		g_thePathingManager->theNodes[37]->theConnections.push_back(g_thePathingManager->theNodes[38]);
		g_thePathingManager->theNodes[37]->theConnections.push_back(g_thePathingManager->theNodes[44]);
		g_thePathingManager->theNodes[37]->theConnections.push_back(g_thePathingManager->theNodes[45]);
		g_thePathingManager->theNodes[37]->theConnections.push_back(g_thePathingManager->theNodes[46]);
		//38 = 29,30,31,37,->9,45,46,47
		g_thePathingManager->theNodes[38]->theConnections.push_back(g_thePathingManager->theNodes[29]);
		g_thePathingManager->theNodes[38]->theConnections.push_back(g_thePathingManager->theNodes[30]);
		g_thePathingManager->theNodes[38]->theConnections.push_back(g_thePathingManager->theNodes[31]);
		g_thePathingManager->theNodes[38]->theConnections.push_back(g_thePathingManager->theNodes[37]);
		g_thePathingManager->theNodes[38]->theConnections.push_back(g_thePathingManager->theNodes[39]);
		g_thePathingManager->theNodes[38]->theConnections.push_back(g_thePathingManager->theNodes[45]);
		g_thePathingManager->theNodes[38]->theConnections.push_back(g_thePathingManager->theNodes[46]);
		g_thePathingManager->theNodes[38]->theConnections.push_back(g_thePathingManager->theNodes[47]);
		//39 = 30,31,28,46,->7
		g_thePathingManager->theNodes[39]->theConnections.push_back(g_thePathingManager->theNodes[30]);
		g_thePathingManager->theNodes[39]->theConnections.push_back(g_thePathingManager->theNodes[31]);
		g_thePathingManager->theNodes[39]->theConnections.push_back(g_thePathingManager->theNodes[38]);
		g_thePathingManager->theNodes[39]->theConnections.push_back(g_thePathingManager->theNodes[46]);
		g_thePathingManager->theNodes[39]->theConnections.push_back(g_thePathingManager->theNodes[47]);
	}

	{
		//40 = 32,33,41,
		g_thePathingManager->theNodes[40]->theConnections.push_back(g_thePathingManager->theNodes[32]);
		g_thePathingManager->theNodes[40]->theConnections.push_back(g_thePathingManager->theNodes[33]);
		g_thePathingManager->theNodes[40]->theConnections.push_back(g_thePathingManager->theNodes[41]);
		//41 = 32,33,34,40,->2
		g_thePathingManager->theNodes[41]->theConnections.push_back(g_thePathingManager->theNodes[32]);
		g_thePathingManager->theNodes[41]->theConnections.push_back(g_thePathingManager->theNodes[33]);
		g_thePathingManager->theNodes[41]->theConnections.push_back(g_thePathingManager->theNodes[34]);
		g_thePathingManager->theNodes[41]->theConnections.push_back(g_thePathingManager->theNodes[40]);
		g_thePathingManager->theNodes[41]->theConnections.push_back(g_thePathingManager->theNodes[42]);
		//42 = 33,34,35,41,->3
		g_thePathingManager->theNodes[42]->theConnections.push_back(g_thePathingManager->theNodes[33]);
		g_thePathingManager->theNodes[42]->theConnections.push_back(g_thePathingManager->theNodes[34]);
		g_thePathingManager->theNodes[42]->theConnections.push_back(g_thePathingManager->theNodes[35]);
		g_thePathingManager->theNodes[42]->theConnections.push_back(g_thePathingManager->theNodes[41]);
		g_thePathingManager->theNodes[42]->theConnections.push_back(g_thePathingManager->theNodes[43]);
		//43 = 34,35,36,42,->4
		g_thePathingManager->theNodes[43]->theConnections.push_back(g_thePathingManager->theNodes[34]);
		g_thePathingManager->theNodes[43]->theConnections.push_back(g_thePathingManager->theNodes[35]);
		g_thePathingManager->theNodes[43]->theConnections.push_back(g_thePathingManager->theNodes[36]);
		g_thePathingManager->theNodes[43]->theConnections.push_back(g_thePathingManager->theNodes[42]);
		g_thePathingManager->theNodes[43]->theConnections.push_back(g_thePathingManager->theNodes[44]);
		//44 = 35,36,37,43,->5
		g_thePathingManager->theNodes[44]->theConnections.push_back(g_thePathingManager->theNodes[35]);
		g_thePathingManager->theNodes[44]->theConnections.push_back(g_thePathingManager->theNodes[36]);
		g_thePathingManager->theNodes[44]->theConnections.push_back(g_thePathingManager->theNodes[37]);
		g_thePathingManager->theNodes[44]->theConnections.push_back(g_thePathingManager->theNodes[43]);
		g_thePathingManager->theNodes[44]->theConnections.push_back(g_thePathingManager->theNodes[45]);
		//45 = 36,37,38,44,->6
		g_thePathingManager->theNodes[45]->theConnections.push_back(g_thePathingManager->theNodes[36]);
		g_thePathingManager->theNodes[45]->theConnections.push_back(g_thePathingManager->theNodes[37]);
		g_thePathingManager->theNodes[45]->theConnections.push_back(g_thePathingManager->theNodes[38]);
		g_thePathingManager->theNodes[45]->theConnections.push_back(g_thePathingManager->theNodes[44]);
		g_thePathingManager->theNodes[45]->theConnections.push_back(g_thePathingManager->theNodes[46]);
		//46 = 37,38,39,45,->7
		g_thePathingManager->theNodes[46]->theConnections.push_back(g_thePathingManager->theNodes[37]);
		g_thePathingManager->theNodes[46]->theConnections.push_back(g_thePathingManager->theNodes[38]);
		g_thePathingManager->theNodes[46]->theConnections.push_back(g_thePathingManager->theNodes[39]);
		g_thePathingManager->theNodes[46]->theConnections.push_back(g_thePathingManager->theNodes[45]);
		g_thePathingManager->theNodes[46]->theConnections.push_back(g_thePathingManager->theNodes[47]);
		//47 = 38,39,46	   ->
		g_thePathingManager->theNodes[47]->theConnections.push_back(g_thePathingManager->theNodes[38]);
		g_thePathingManager->theNodes[47]->theConnections.push_back(g_thePathingManager->theNodes[39]);
		g_thePathingManager->theNodes[47]->theConnections.push_back(g_thePathingManager->theNodes[46]);
	}

	return;
}

//Name:		reconstructPath
//Purpose:	Works backwards from the end node and fills a vector with the path taken.
//Return:	void
void reconstructPath(std::map<unsigned int, cPathingNode*> cameFrom, cPathingNode* currNode, std::vector<cPathingNode*>& theReturnNodes, unsigned int startingNodeID) {
	cPathingNode* tempNodeToAdd = new cPathingNode();
	copyNodePointerToNewNode(currNode, *tempNodeToAdd);

	theReturnNodes.push_back(tempNodeToAdd);
	cPathingNode* tempNodePointer = currNode;
	unsigned int previousPositionIndex = 0;
	//if the current nodes id is equal to the start then we have finished reconstructing the path
	while (tempNodePointer->nodeId != startingNodeID)
	{
		//set the node id to find
		previousPositionIndex = tempNodePointer->nodeId;
		//create a new node to add
		tempNodeToAdd = new cPathingNode();
		//find the node by id
		tempNodePointer = cameFrom[previousPositionIndex];
		//populate the new node
		copyNodePointerToNewNode(tempNodePointer, *tempNodeToAdd);
		//add the new node to the vector
		theReturnNodes.push_back(tempNodeToAdd);
	}

	return;
}

//Name:		removeNodeFromOpen
//Purpose:	Removes a node from the open vector.
//Return:	void
void removeNodeFromOpen(cPathingNode* theNode, std::vector<cPathingNode*>& vecTheNodes) {
	for (std::vector<cPathingNode*>::iterator it = vecTheNodes.begin(); it != vecTheNodes.end();)
	{
		if ((*it)->nodeId == theNode->nodeId)
		{
			it = vecTheNodes.erase(it);
		}
		else
			++it;
	}
}

//Name:		copyNodePointerToNewNode
//Purpose:	Copies the info from one node* to another without copying the memory address.
//Return:	void
void copyNodePointerToNewNode(cPathingNode* nodeToCopy, cPathingNode& theNewNode) {
	theNewNode.isActive = nodeToCopy->isActive;
	theNewNode.nodeId = nodeToCopy->nodeId;
	theNewNode.position = nodeToCopy->position;
	theNewNode.theConnections = nodeToCopy->theConnections;
	theNewNode.value = nodeToCopy->value;
}