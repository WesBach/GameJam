#ifndef _sMeshDrawInfo_HG_
#define _sMeshDrawInfo_HG_

#include "sTextureBindBlendInfo.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <vector>

// Note: Meshes have a separate orientation and offset from
//	the object, in case you want the mesh(es) to be
//	loaded in different alignment from the game object.
//  If the object alignment is the same as the mesh
//	alignment, then don't set the orientation and offset
//	in the mesh information.
struct sMeshDrawInfo
{
public:
	sMeshDrawInfo() :
		drawID(0), scale(1.0f), bIsVisible(true),
		bDrawAsWireFrame(false), bDisableBackFaceCulling(false),
		offset(glm::vec3(0.0f, 0.0f, 0.0f)),
		prevOffset(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_meshQOrientation(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f))),
		debugDiffuseColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
		bDisableDepthTest(false),
		globalAlpha(1.0f),
		bIsSkyBoxObject(false),
		reflectBlendRatio(0.0f), refractBlendRatio(0.0f), coefficientRefract(1.0f),
		minXYZ_from_SM_Bones(glm::vec3(0.0f)),
		maxXYZ_from_SM_Bones(glm::vec3(0.0f))
	{};
	std::string name;
	unsigned int drawID;
	float scale;
	glm::vec3 offset;
	glm::vec3 prevOffset;
	bool bIsVisible;			// Render ignores objects that are false (not child objects could still be visible)
	bool bDrawAsWireFrame;		// Polygon mode is LINES for all meshes
	bool bUseDebugColour;
	glm::vec4 debugDiffuseColour;	// Colour used if wireframe
	bool bDisableBackFaceCulling;	// Draw both sides if true
	bool bDisableDepthTest;

	bool bIsSkyBoxObject;		// Just texture, no lighting

	float globalAlpha;	// Used for entire object

	float reflectBlendRatio;
	float refractBlendRatio;
	float coefficientRefract;
	// Textures only have name and blending info
	std::vector<sTextureBindBlendInfo> vecMehs2DTextures;
	std::vector<sTextureBindBlendInfo> vecMeshCubeMaps;

	// Extent Values for skinned mesh
	// These can be updated per frame, from the "update skinned mesh" call
	glm::vec3 minXYZ_from_SM_Bones;
	glm::vec3 maxXYZ_from_SM_Bones;
	// Store all the bones for this model, buing updated per frame
	std::vector< glm::mat4x4 > vecObjectBoneTransformation;

private:
	glm::quat m_meshQOrientation;
public:
	glm::quat getQOrientation(void) { return this->m_meshQOrientation; };
	void setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);
};

#endif
