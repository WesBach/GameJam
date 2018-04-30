#include "cPowerUp.h"
#include "cGameObject.h"

ePowerUpType cPowerUp::getPowerUpType() {
	return this->mPowerUpType;
}

eModifierType cPowerUp::getModifierType() {
	return this->mModifierType;
}

void cPowerUp::setModifierType(eModifierType type) {
	this->modifierValue = type;
}

cPowerUp::cPowerUp(eModifierType type, ePowerUpType powerupType,cGameObject* theObject) {
	this->modifierValue = type;
	this->mPowerUpType = powerupType;
	this->mModifierType = type;
	this->thePowerUp = theObject;
}

