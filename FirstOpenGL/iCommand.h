#ifndef _iCommand_HG_
#define _iCommand_HG_

class iCommand {
public:
	virtual void update(double deltaTime) = 0;
	virtual bool getDone() = 0;
};
#endif // !_iCommand_HG_
