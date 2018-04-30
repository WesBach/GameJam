#ifndef _sVertex_xyz_rgba_n_uv2_bt_skinnedMesh_HG_
#define _sVertex_xyz_rgba_n_uv2_bt_skinnedMesh_HG_

// This represents the vertex as used in the vertex layout.
// It's also the vertex representation used in cMesh
// (it replaces the cVertex in the cVAOMeshManager and the cVertex_xyz_rgb_n structures)

// This matches the shader layout for the vertex
#include <cstring>		// for memset

static const unsigned int NUMBEROFBONES = 4;

struct sVertex_xyz_rgba_n_uv2_bt_4Bones
{
	sVertex_xyz_rgba_n_uv2_bt_4Bones() :
		x(0.0f), y(0.0f), z(0.0f),
		r(0.0f), g(0.0f), b(0.0f), a(1.0f),		// Note alpha is 1.0
		nx(0.0f), ny(0.0f), nz(0.0f),
		u1(0.0f), v1(0.0f),
		u2(0.0f), v2(0.0f),
		tx(0.0f), ty(0.0f), tz(0.0f),
		bx(0.0f), by(0.0f), bz(0.0f)
	{
//#ifdef _DEBUG
		memset(this->boneID, 0, sizeof(unsigned int) * NUMBEROFBONES);
		memset(this->boneWeights, 0, sizeof(float) * NUMBEROFBONES);
//#endif // DEBUG
	};	
	// Destructor isn't really needed here
	//~sVertex_xyz_rgba_n_uv2_bt_skinnedMesh();	

	float x, y, z;									// 12
	float r, g, b, a;								// 16 (28)
	float nx, ny, nz;								// 12 (40)
	float u1, v1;									// 8  (48)
	float u2, v2;									// 8  (56)
	float tx, ty, tz;	// tangent					// 12 (68)
	float bx, by, bz;	// bi-normal				// 12 (80)
	// For the 4 bone skinned mesh information
	float boneID[NUMBEROFBONES]; 		// New		// starts at 80
	float boneWeights[NUMBEROFBONES];	// New		// starts at 96
};


#endif


