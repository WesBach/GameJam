#ifndef _cAnimationManager_HG_
#define _cAnimationManager_HG_
#include "cAnimationState.h"
#include "assimp\cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

class cAnimationManager {
public:
	cAnimationManager() {
		pSimpleSkinnedMesh = NULL;
		pAniState = NULL;
	}

	~cAnimationManager() {
		delete this->pSimpleSkinnedMesh;
		delete this->pAniState;
	}
	cSimpleAssimpSkinnedMesh*	pSimpleSkinnedMesh;
	cAnimationState*			pAniState;
	std::map<std::string, std::pair<bool, cAnimationState::sStateDetails>> animationStateMap;
	std::vector< cAnimationState::sStateDetails> animationsToRun;
	void setOtherAnimationsToNotRunning(std::vector<cAnimationState::sStateDetails> animations);

};

#endif // !_cAnimationManager_HG_
