#ifndef _cPowerUp_HG_
#define _cPowerUp_HG_


enum ePowerUpType {
	POWERUP_HEALTH = 0,
	POWERUP_RANGE_INCREASE,
	POWERUP_DAMAGE_INCREASE,
	POWERUP_SPEEDUP,
	POWERUP_UNKNOWN
};

enum eModifierType {
	MODIFIER_HEALTH = 30,
	MODIFIER_SPEEDUP = 4,
	MODIFIER_RANGE_INCREASE = 2,
	MODIFIER_DAMAGE_INCREASE = 10
};

class cGameObject;

class cPowerUp {
public:
	cPowerUp(eModifierType type, ePowerUpType pickupType,cGameObject* theObject);
	ePowerUpType getPowerUpType();
	eModifierType getModifierType();
	void setModifierType(eModifierType type);
	//will be different for each type
	float modifierValue;
	cGameObject* thePowerUp;
private:
	ePowerUpType mPowerUpType;
	eModifierType mModifierType;
};

#endif // !_cPickup_HG
