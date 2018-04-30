#ifndef _cSimpleAssimpSkinnedMesh_HG
#define _cSimpleAssimpSkinnedMesh_HG

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <vector>
#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

//#include "vert_XYZ_RGBA_N_STU_TanBi_4Bones.h"
#include "../sVertex_xyz_rgba_n_uv2_bt_4Bones.h"

// Used by the game object and the rendering
#include "../cMesh.h"			// For loading into the VAO manager

class cSimpleAssimpSkinnedMesh 
{
private:
	static const int MAX_BONES_PER_VERTEX = 4;

	struct sVertexBoneData
	{
		//std::array<unsigned int, MAX_BONES_PER_VERTEX> ids;
		std::array<float, MAX_BONES_PER_VERTEX> ids;
		std::array<float, MAX_BONES_PER_VERTEX> weights;

		void AddBoneData(unsigned int BoneID, float Weight);
	};

	struct sBoneInfo
	{
		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;
		glm::mat4 ObjectBoneTransformation;
	};

public:
	// Mesh and VAO and VBO infor
	//unsigned int m_VBO_ID;				// Vertex buffer object 
	unsigned int m_numberOfVertices;
	//unsigned int m_indexBuf_ID;			// Index buffer referring to VBO
	unsigned int m_numberOfIndices;
	unsigned int m_numberOfTriangles;

	cSimpleAssimpSkinnedMesh(void);
	virtual ~cSimpleAssimpSkinnedMesh(void);

	bool LoadMeshFromFile(const std::string &filename);		// mesh we draw
	bool LoadMeshAnimation(const std::string &filename);	// Only want animations

	std::string fileName;
	std::string friendlyName;

	const aiScene* pScene;		// "pretty" mesh we update and draw
	Assimp::Importer mImporter;

	std::map< std::string /*animationfile*/,
		      const aiScene* > mapAnimationNameTo_pScene;		// Animations

	glm::mat4 AIMatrixToGLMMatrix(const aiMatrix4x4 &mat);

	// Looks in the animation map and returns the total time
	float FindAnimationTotalTime( std::string animationName );

	cMesh* CreateMeshObjectFromCurrentModel( unsigned int meshIndex = 0 );

	void BoneTransform(float TimeInSeconds,
		std::string animationName,		// Now we can pick the animation
		std::vector<glm::mat4> &FinalTransformation,
		std::vector<glm::mat4> &Globals,
		std::vector<glm::mat4> &Offsets,
		glm::vec3& handPositionL, glm::vec3& handPositionR);

	unsigned int GetNums(void) const { return this->mNumBones; }

	float GetDuration(void);
	
	std::vector<sVertexBoneData> vecVertexBoneData;	//Bones;
	glm::mat4 mGlobalInverseTransformation;


	bool Initialize(void);
	
	void CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, aiQuaternion& out);
	void CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D& out);
	void CalcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D& out);

	void CalcGLMInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::quat& out);
	void CalcGLMInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);
	void CalcGLMInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);

	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

	const aiNodeAnim* FindNodeAnimationChannel(const aiAnimation* pAnimation, aiString nodeOrBoneName);
	
	void ReadNodeHeirarchy(float AnimationTime,
		std::string animationName,
		const aiNode* pNode,
		const glm::mat4 &ParentTransformMatrix,
		glm::vec3& posOffsets, glm::vec3& handPositionR);

	void LoadBones(const aiMesh* Mesh, std::vector<sVertexBoneData> &Bones);
	void ShutErDown(void);

	std::map<std::string /*BoneName*/, unsigned int /*BoneIndex*/> m_mapBoneNameToBoneIndex;	//mMapping;
	std::vector<sBoneInfo> mBoneInfo;
	unsigned int mNumBones;	//mNums;
};

#endif
