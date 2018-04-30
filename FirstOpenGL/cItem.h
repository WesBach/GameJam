#ifndef _cItem_HG_
#define _cItem_HG_
#include <string>
#include "sTextureBindBlendInfo.h"
#include "sObjectWorldInfo.h"
#include <vector>

class cItem {
public:
	cItem();
	~cItem() { delete drawInfo; }

	std::string friendlyName;
	unsigned long itemID;
	float weight;

	sObjectWorldInfo* drawInfo;
	std::vector<sTextureBindBlendInfo> vecMeshTextures;	
};

#endif // !_cItem_HG_
