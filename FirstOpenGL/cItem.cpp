#include "cItem.h"

cItem::cItem() {
	//create the new draw info and set the default values
	this->drawInfo = new sObjectWorldInfo();

	this->friendlyName = "";
	this->itemID = 0;
	this->weight = 0.0f;
}

