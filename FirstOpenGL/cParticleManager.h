#ifndef _cParticleManager_HG_
#define _cParticleManager_HG_
#include "cEmitter.h"
class cEnemy;
class cPlayer;

class cParticleManager {
public:
	cParticleManager();
	~cParticleManager();
	void setEnemyEmitterToActive(int index);
	std::vector<cEmitter*>& getActiveEmitters();
	void deactivateEmitter(int index);
	void clearAllEmitters();
	void cParticleManager::clearActiveEmitter();
	void drawActiveParticles(int shaderId);
	void createEmitters(int numEmitters);
	void connectEmittersWithEntities(std::vector<cEnemy>& enemies);
	void setPlayerEmitterToActive();
	void setPlayerEmitterToInactive();
	void updateEmitterPositions(std::vector<cEnemy>& enemies, cPlayer* thePlayer);
	void updateLivingParticles(float deltaTime);
private:
	void addEmitter(cEmitter* emitter);
	std::vector<cEmitter*> mEnemyEmitters;
	cEmitter* mPlayerEmitter;
	std::vector<cEmitter*> mActiveEmitters;
};

#endif // ! _cParticleManager_HG_
