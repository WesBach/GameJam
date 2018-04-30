#include "cInventory.h"

cInventory::cInventory() {

}

cInventory::~cInventory() {

}

bool cInventory::addItem(cItem* theItem) {
	//check to make sure the item doesn't already exist
	bool exists = false;
	for (int i = 0; i < this->vecItems.size(); i++)
	{
		if (this->vecItems[i] == theItem)
		{
			exists = true;
			break;
		}
	}

	//if it exists return false
	if (exists)
		return false;
	else
		this->vecItems.push_back(theItem);

	//everything worked
	return true;
}

bool cInventory::removeItem(cItem* theItem) {

	for (std::vector<cItem*>::iterator it = this->vecItems.begin(); it != this->vecItems.end(); it++)
	{
		//if we find the item
		if (*it == theItem)
		{
			//remove from the vector
			it = this->vecItems.erase(it);
			return true;
		}
	}

	return false;
}

cItem* cInventory::getItem(std::string name) {
	//check to see if the name is in the vector
	for (unsigned int i = 0; i < this->vecItems.size(); i++)
	{
		//if it is return that item
		if (this->vecItems[i]->friendlyName == name)
		{
			return this->vecItems[i];
			break;
		}
	}
}

cItem* cInventory::getItem(unsigned long id) {
	//check to see if the id is in the vector
	for (unsigned int i = 0; i < this->vecItems.size(); i++)
	{
		//if it is return it
		if (this->vecItems[i]->itemID == id)
		{
			return this->vecItems[i];
			break;
		}
	}
}

