#ifndef _PathingUtilities_HG_
#define _PathingUtilities_HG_
	
#include "cPathingNode.h"
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <glm\glm.hpp>
#include <string>

class cGameObject;

void findShortestPathAStar(int startingIndex, int endingIndex,std::vector<cPathingNode*>& theShortestPath);

void getNodeWithShortestFScoreFromOpen(std::vector<cPathingNode*>& theOpenNodes, std::map<unsigned int, std::pair<cPathingNode*, float>>& fScores,cPathingNode& current);

bool sortShortestBySecond(std::pair<cPathingNode*, float>& pairA, std::pair<cPathingNode*, float>& pairB);

bool isPresent(const cPathingNode* theNode, const std::vector<cPathingNode*>& theNodes);

void removeNodeFromOpen(cPathingNode* theNode, std::vector<cPathingNode*>& vecTheNodes);
void setNodeConnections();

void reconstructPath(std::map<unsigned int, cPathingNode*> cameFrom, cPathingNode* currNode, std::vector<cPathingNode*>& theReturnNodes,unsigned int theStartId);

void copyNodePointerToNewNode(cPathingNode* nodeToCopy, cPathingNode& theNewNode);

#endif // 
