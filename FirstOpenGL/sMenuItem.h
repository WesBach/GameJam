#ifndef _sMenuItem_HG_
#define _sMenuItem_HG_
#include <vector>
#include <glm\vec3.hpp>
#include "sTextureBindBlendInfo.h"

//menu items to draw
struct sMenuItem {
	sMenuItem() {
		width = 0.0f;
		height = 0.0f;
		backgroundColor = glm::vec3(0.0f);
	}
	float width;
	float height;
	glm::vec3 backgroundColor;
	std::vector<sTextureBindBlendInfo> vecMeshTextures;
};

#endif // !_sMenuItem_HG_
