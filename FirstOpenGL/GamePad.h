#ifndef  _GamePad_HG_
#define _GamePad_HG_

class GamePad {
public:
	GamePad();
	void resolveJoystickInteractions();
	void getIsConnected();
	void getAxesArray();
	void updateShoot();
private:
	int isPresent;
	//axes
	const float* axes;
	int axesCount;	
	//buttons
	int buttonCount;
	const unsigned char* buttons;
	bool buttonsAndAxesPopulated;

	void handleAxesUpdate(int index);
	void handleButtonUpdate(int index);
	bool shoot;
};

#endif // ! _GamePad_HG_

