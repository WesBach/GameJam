#include "cAnimationManager.h"

void cAnimationManager::setOtherAnimationsToNotRunning(std::vector<cAnimationState::sStateDetails> animations) {
	std::map<std::string, std::pair<bool, cAnimationState::sStateDetails>>::iterator it = this->animationStateMap.begin();

	for (; it != this->animationStateMap.end(); it++){
		//set all animation states to false
		it->second.first = false;
	}

	//set the appropriate animations to true
	for (int i = 0; i < animations.size(); i++) {
		this->animationStateMap[animations[i].friendlyName].first = true;
	}
}