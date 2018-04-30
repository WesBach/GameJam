#ifndef _cInventory_HG_
#define _cInventory_HG_

#include "cItem.h"

class cInventory {
public:
	cInventory();
	~cInventory();

	bool addItem(cItem* theItem);
	bool removeItem(cItem* theItem);

	cItem* getItem(std::string name);
	cItem* getItem(unsigned long id);

private:
	std::vector<cItem*> vecItems;
};

#endif // !_cInventory_HG_
