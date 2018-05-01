#include "RenderUtilities.h"
#include "cShaderManager.h"
#include "cLightManager.h"
//#include "Camera.h"
#include "cMesh.h"
#include "cVAOMeshManager.h"
#include "Utilities.h"
#include "CTextureManager.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "cAnimationManager.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
extern cShaderManager*	g_pShaderManager;		// Heap, new (and delete)
extern cLightManager*	g_pLightManager;
//extern Camera* g_pTheCamera;
extern bool drawDebugInfo;
extern cVAOMeshManager* g_pVAOManager;
extern glm::vec3 g_cameraXYZ;
extern glm::vec3 g_cameraTarget_XYZ;
extern std::vector< cGameObject* >  g_vecGameObjects;
extern glm::mat4 g_lightSpaceMatrix;
extern glm::mat4 g_lightProjection;
extern glm::mat4 g_lightView;
extern unsigned int SHADOW_WIDTH;
extern unsigned int SHADOW_HEIGHT;
extern bool bUseFog;
//TODO::
//will have to delete later
cMesh* tempMesh = new cMesh();
float time = 5.f;

bool renderScene(std::vector<cGameObject*> theObjects, GLFWwindow* theWindow, int type, float deltaTime) {
	float ratio;
	int width, height;
	glm::mat4x4 matProjection;			// was "p"
	glm::mat4 matView;

	glfwGetFramebufferSize(theWindow, &width, &height);

	if (type == 0)
	{
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
	}
	else if (type == 3)
	{
		ratio = SHADOW_WIDTH / (float)SHADOW_HEIGHT;
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	}

	::g_pShaderManager->useShaderProgram("mySexyShader");
	GLint curShaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");

	// View or "camera" matrix
	// (for the whole scene)
	::g_pLightManager->CopyLightInformationToCurrentShader();

	if (type != 3) //not the shadow pass
	{
		matProjection = glm::perspective(0.6f,			// FOV
			ratio,		// Aspect ratio
			1.0f,		// Near (as big as possible)
			10000.0f);	// Far (as small as possible)
		matView = glm::lookAt(g_cameraXYZ,						// "eye" or "camera" position
			g_cameraTarget_XYZ,		// "At" or "target"
			glm::vec3(0.0f, 1.0f, 0.0f));	// "up" vector
	}
	else
	{
		matProjection = g_lightProjection;
		matView = g_lightView;
	}

	GLint uniLoc_mView = glGetUniformLocation(curShaderID, "mView");
	GLint uniLoc_mProjection = glGetUniformLocation(curShaderID, "mProjection");
	GLint uniLoc_mLightSpaceMatrix = glGetUniformLocation(curShaderID, "lightSpaceMatrix");

	glUniformMatrix4fv(uniLoc_mView, 1, GL_FALSE,
		(const GLfloat*)glm::value_ptr(matView));
	glUniformMatrix4fv(uniLoc_mProjection, 1, GL_FALSE,
		(const GLfloat*)glm::value_ptr(matProjection));

	glUniformMatrix4fv(uniLoc_mLightSpaceMatrix, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(g_lightSpaceMatrix));

	// Enable blend ("alpha") transparency for the scene
	// NOTE: You "should" turn this OFF, then draw all NON-Transparent objects
	//       Then turn ON, sort objects from far to near ACCORDING TO THE CAMERA
	//       and draw them
	glEnable(GL_BLEND);		// Enables "blending"
							//glDisable( GL_BLEND );
							// Source == already on framebuffer
							// Dest == what you're about to draw
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw the scene
	unsigned int sizeOfVector = (unsigned int)theObjects.size();	//*****//
	for (int index = 0; index != sizeOfVector; index++)
	{
		// This is the top level vector, so they are all "parents"
		DrawObject(theObjects[index], curShaderID, deltaTime, false/*used only for instanced objects (particles at the moment)*/, 0);
	}//for ( int index = 0...

	return true;
}

void DrawObject(cGameObject* pTheGO, GLint curShaderProgramID, float deltaTime, bool instanced, int numInstancedParticles)
{
	// Is there a game object?
	if (pTheGO == 0)	//if ( ::g_GameObjects[index] == 0 )
	{	// Nothing to draw
		return;		// Skip all for loop code and go to next
	}

	g_pVAOManager->lookupMeshFromName(pTheGO->meshName, *tempMesh);

	if (drawDebugInfo) {
		//build the bounding boxes
		if (pTheGO->bHasAABB)
		{
			pTheGO->theBoundingBox->update(tempMesh, pTheGO->position, pTheGO->scale);
			pTheGO->theBoundingBox->buildBoundingBox(pTheGO->scale);
		}
	}

	glm::mat4x4 mModel = glm::mat4x4(1.0f);	//		mat4x4_identity(m);

	//glm::mat4 matRreRotZ = glm::mat4x4(1.0f);
	//matRreRotZ = glm::rotate(matRreRotZ, pTheGO->orientation.z,
	//	glm::vec3(0.0f, 0.0f, 1.0f));
	//mModel = mModel * matRreRotZ;

	glm::mat4 trans = glm::mat4x4(1.0f);
	trans = glm::translate(trans,
		pTheGO->position);
	mModel = mModel * trans;

	glm::mat4 matPostRotZ = glm::mat4x4(1.0f);
	matPostRotZ = glm::rotate(matPostRotZ, pTheGO->orientation2.z,
		glm::vec3(0.0f, 0.0f, 1.0f));
	mModel = mModel * matPostRotZ;

	glm::mat4 matPostRotY = glm::mat4x4(1.0f);
	matPostRotY = glm::rotate(matPostRotY, pTheGO->orientation2.y,
		glm::vec3(0.0f, 1.0f, 0.0f));
	mModel = mModel * matPostRotY;

	glm::mat4 matPostRotX = glm::mat4x4(1.0f);
	matPostRotX = glm::rotate(matPostRotX, pTheGO->orientation2.x,
		glm::vec3(1.0f, 0.0f, 0.0f));
	mModel = mModel * matPostRotX;
	// TODO: add the other rotation matrix (i.e. duplicate code above)
	float finalScale = pTheGO->scale;

	glm::mat4 matScale = glm::mat4x4(1.0f);
	matScale = glm::scale(matScale,
		glm::vec3(finalScale,
			finalScale,
			finalScale));
	mModel = mModel * matScale;

	GLuint uniLoc_mModel = glGetUniformLocation(curShaderProgramID, "mModel");;
	glUniformMatrix4fv(uniLoc_mModel, 1, GL_FALSE,
		(const GLfloat*)glm::value_ptr(mModel));

	glm::mat4 mWorldInTranpose = glm::inverse(glm::transpose(mModel));

	GLuint uniLoc_materialDiffuse = glGetUniformLocation(curShaderProgramID, "materialDiffuse");
	glUniform4f(uniLoc_materialDiffuse,
		pTheGO->diffuseColour.r,
		pTheGO->diffuseColour.g,
		pTheGO->diffuseColour.b,
		pTheGO->diffuseColour.a);

	//GLint uniLoc_mLightSpaceMatrix = glGetUniformLocation(curShaderProgramID, "lightSpaceMatrix");
	//glUniformMatrix4fv(uniLoc_mLightSpaceMatrix, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(g_lightSpaceMatrix));

	///////////////////////////////////////

	//Set ALL the samplers to something (so they don't point to GL_TEXTURE0)
	QnDTexureSamplerUtility::LoadUniformLocationsIfNeeded(curShaderProgramID);
	// There's no way we have 999 texture units...
	QnDTexureSamplerUtility::setAllSamplerUnitsToInvalidNumber(999);
	QnDTexureSamplerUtility::clearAllBlendValuesToZero();

	// Now set our samplers, and blend function to something
	// Basic texture binding setup (assign all loaded textures to samplers)
	g_pTextureManager->UpdateTextureBindingsByTextureNameSimple();
	std::map<std::string, CTexUnitInfoBrief> mapTexNameToTexUnit;

	g_pTextureManager->GetBoundTextureUnitsByTextureNames(mapTexNameToTexUnit);

	//// Now look up what textures our object is using and set the samplers
	QnDTexureSamplerUtility::SetSamplersForMeshTextures(*pTheGO, mapTexNameToTexUnit);

	GLint skyColorUniform = glGetUniformLocation(curShaderProgramID, "skycolor");
	GLint bUseFogUniform = glGetUniformLocation(curShaderProgramID, "useFog");
	GLint texHeightMap_UniLoc = glGetUniformLocation(curShaderProgramID, "textHeightMap");
	GLint bIsHeightMap_UniLoc = glGetUniformLocation(curShaderProgramID, "isHeightMapped");
	GLint uniLoc_bIsDebugWireFrameObject = glGetUniformLocation(curShaderProgramID, "bIsDebugWireFrameObject");
	GLint bIsNoise_UniLoc = glGetUniformLocation(curShaderProgramID, "isNoiseGenerating");
	GLint fDeltaTime_uniloc = glGetUniformLocation(curShaderProgramID, "deltaTime");
	GLint uvOffsetForWaves = glGetUniformLocation(curShaderProgramID, "noiseOffset");
	GLint uniLoc_bIsReflectRefract = glGetUniformLocation(curShaderProgramID, "isReflectRefract");
	GLint uniLoc_bRefractCoefficient = glGetUniformLocation(curShaderProgramID, "coefficientRefract");
	GLint uniLoc_bReflectRatio = glGetUniformLocation(curShaderProgramID, "reflectBlendRatio");
	GLint uniLoc_bRefractRatio = glGetUniformLocation(curShaderProgramID, "refractBlendRatio");
	GLint uniLoc_bIsSkyBoxObject = glGetUniformLocation(curShaderProgramID, "isASkyBox");
	GLint uniloc_useDiffuse = glGetUniformLocation(curShaderProgramID, "IsDiffuse");
	//IsDiffuse

	if (pTheGO->bIsSkyBoxObject)
	{
		glUniform1f(uniLoc_bIsSkyBoxObject, GL_TRUE);
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glUniform1f(uniLoc_bIsSkyBoxObject, GL_FALSE);
	}

	glUniform1f(uniLoc_bReflectRatio, pTheGO->reflectRatio);
	glUniform1f(uniLoc_bIsReflectRefract, pTheGO->bIsReflectRefract);
	glUniform1f(uniLoc_bRefractRatio, pTheGO->refractRatio);

	//coefficientRefract
	if (pTheGO->refractRatio > 0.0f) {
		glUniform1f(uniLoc_bRefractCoefficient, 1.f);
	}
	else {
		glUniform1f(uniLoc_bRefractCoefficient, 0.0f);
	}

	// Wireframe?
	if (pTheGO->bIsWireFrame)
	{
		glUniform1f(uniLoc_bIsDebugWireFrameObject, 1.0f);	// TRUE
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Default
	}
	else
	{
		glUniform1f(uniLoc_bIsDebugWireFrameObject, 0.0f);	// FALSE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Default
	}

	if (pTheGO->vecMehs2DTextures.size() <= 0) {
		//uniloc_useDiffuse
		glUniform1f(uniloc_useDiffuse, GL_TRUE);
	}
	else {
		glUniform1f(uniloc_useDiffuse, GL_FALSE);
	}

	// HACK: Check for the mesh name
	if (pTheGO->heightMap != NULL)
	{	// YES! It's the height map!
		// This is the TEXTURE ID, NOT the "texture UNIT" ID
		//GLint textureNameID  = ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->heightMap->name);
		////std::map<std::string, CTexUnitInfoBrief>::iterator itTextUnitInfo	= mapTexAndUnitInfo.find(object.vecMehs2DTextures[samplerIndex].name);
		//std::map<std::string, CTextureManager::CTextureUnitInfo>::iterator itTextUnitInfo = ::g_pTextureManager->m_map_TexNameToTexUnit.find(pTheGO->heightMap->name);
		////// Pick a texture unit we are not likely using elswhere
		//glActiveTexture(itTextUnitInfo->second.texUnitNumOffsetBy_GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, itTextUnitInfo->second.texUnitNumber);

		//int* data;
		//glGetIntegerv(GL_TEXTURE_BINDING_2D, data);

		////// Now assign the sampler to the texture Unit
		//glUniform1f(texHeightMap_UniLoc, itTextUnitInfo->second.texUnitNumOffsetBy_GL_TEXTURE0);
		glUniform1f(bIsHeightMap_UniLoc, GL_TRUE);
	}
	else
	{
		glUniform1f(bIsHeightMap_UniLoc, GL_FALSE);
	}

	//deltaTime

	if (pTheGO->isNoiseGenerating) {
		glUniform1f(bIsNoise_UniLoc, GL_TRUE);
		glUniform1f(fDeltaTime_uniloc, deltaTime);
		//get the random texture index
		//int index = getRandInRange(0, (int)(::g_pTextureManager->m_map_TexNameToTexture.size()) - 1);
		//int counter = 0;

		//std::string name;
		////get that texture name
		//for (std::map<std::string, CTextureFromBMP*>::iterator it = ::g_pTextureManager->m_map_TexNameToTexture.begin(); it != ::g_pTextureManager->m_map_TexNameToTexture.end(); it++) {
		//	if (counter == index) {
		//		name = (*it).second->getTextureName();
		//		break;
		//	}
		//	counter++;
		//}

		////get the texture id
		GLint textureNameID = ::g_pTextureManager->getTextureIDFromTextureName("heightmap.bmp");
		//::g_pTextureManager->getTextureIDFromTextureName(pTheGO->heightMap->name);
		//GLint textureNameID = ::g_pTextureManager->getTextureIDFromTextureName(pTheGO->heightMap->name);
		// Bind the texture to the correct spot
		//glActiveTexture(GL_TEXTURE0 + textureNameID);
		//glBindTexture(GL_TEXTURE_2D, textureNameID);
		// Now assign the sampler to the texture Unit
		//glUniform1f(texHeightMap_UniLoc, textureNameID);
		//set the offset
		glUniform1f(uvOffsetForWaves, time);
		time -= deltaTime / 20.f;

		if (time <= -5.f) {
			time = 5.f;
		}
	}
	else {
		glUniform1f(bIsNoise_UniLoc, GL_FALSE);
	}

	///////////////////////////////////////////

	// ***************************************************
	//    ___  _    _                      _  __  __           _
	//   / __|| |__(_) _ _   _ _   ___  __| ||  \/  | ___  ___| |_
	//   \__ \| / /| || ' \ | ' \ / -_)/ _` || |\/| |/ -_)(_-<| ' \
	//   |___/|_\_\|_||_||_||_||_|\___|\__,_||_|  |_|\___|/__/|_||_|
	//
	GLint UniLoc_IsSkinnedMesh = glGetUniformLocation(curShaderProgramID, "bIsASkinnedMesh");

	if (pTheGO->pAnimationManager != NULL)
	{
		if (pTheGO->pAnimationManager->pAniState != NULL)
		{
			// Calculate the pose and load the skinned mesh stuff into the shader, too
			GLint UniLoc_NumBonesUsed = glGetUniformLocation(curShaderProgramID, "numBonesUsed");
			GLint UniLoc_BoneIDArray = glGetUniformLocation(curShaderProgramID, "bones");

			CalculateSkinnedMeshBonesAndLoad(pTheGO->drawInfo, pTheGO, UniLoc_NumBonesUsed, UniLoc_BoneIDArray);
			glUniform1f(UniLoc_IsSkinnedMesh, GL_TRUE);
		}
	}
	else
	{
		glUniform1f(UniLoc_IsSkinnedMesh, GL_FALSE);
	}

	//GLint UniLoc_IsDepthBuffer = glGetUniformLocation(curShaderProgramID, "bIsADepthBuffer");
	GLint Uniloc_IsToonShaded = glGetUniformLocation(curShaderProgramID, "toonShade");
	glUniform1f(Uniloc_IsToonShaded, pTheGO->bIstoonShaded);

	if (bUseFog) {
		glUniform1f(bUseFogUniform, bUseFog);
		glUniform3f(skyColorUniform, 0.5f, 0.5f, 0.5f);
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	if (pTheGO->vecMeshNames.size() <= 1) {
		std::string meshToDraw = pTheGO->meshName;		//::g_GameObjects[index]->meshName;
		sVAOInfo VAODrawInfo;
		if (::g_pVAOManager->lookupVAOFromName(meshToDraw, VAODrawInfo) == false)
		{	// Didn't find mesh
			return;
		}

		if (instanced) {
			GLint uniloc_isInstanced = glGetUniformLocation(curShaderProgramID, "isInstanced");
			glUniform1f(uniloc_isInstanced, GL_TRUE);

			glBindVertexArray(VAODrawInfo.VAO_ID);
			glDrawElementsInstanced(GL_TRIANGLES,
				VAODrawInfo.numberOfIndices,		// testMesh.numberOfTriangles * 3,	// How many vertex indices
				GL_UNSIGNED_INT,					// 32 bit int
				0, numInstancedParticles);

			glUniform1f(uniloc_isInstanced, GL_FALSE);
		}
		else {
			glBindVertexArray(VAODrawInfo.VAO_ID);
			glDrawElements(GL_TRIANGLES,
				VAODrawInfo.numberOfIndices,		// testMesh.numberOfTriangles * 3,	// How many vertex indices
				GL_UNSIGNED_INT,					// 32 bit int
				0);
		}
	}
	else {
		//for objects with multiple meshes
		for (int i = 0; i < pTheGO->vecMeshNames.size(); i++) {
			std::string meshToDraw = pTheGO->vecMeshNames[i];		//::g_GameObjects[index]->meshName;

			sVAOInfo VAODrawInfo;
			if (::g_pVAOManager->lookupVAOFromName(meshToDraw, VAODrawInfo) == false)
			{	// Didn't find mesh
				return;
			}
			glBindVertexArray(VAODrawInfo.VAO_ID);

			glDrawElements(GL_TRIANGLES,
				VAODrawInfo.numberOfIndices,		// testMesh.numberOfTriangles * 3,	// How many vertex indices
				GL_UNSIGNED_INT,					// 32 bit int
				0);
		}
	}

	// Unbind that VAO
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	return;
}

//****************************************************************************************
//    ___  _    _                      _  __  __           _
//   / __|| |__(_) _ _   _ _   ___  __| ||  \/  | ___  ___| |_
//   \__ \| / /| || ' \ | ' \ / -_)/ _` || |\/| |/ -_)(_-<| ' \
//   |___/|_\_\|_||_||_||_||_|\___|\__,_||_|  |_|\___|/__/|_||_|
//
void CalculateSkinnedMeshBonesAndLoad(sMeshDrawInfo &theMesh, cGameObject* pTheGO,
	unsigned int UniformLoc_numBonesUsed,
	unsigned int UniformLoc_bonesArray)
{
	std::string animationToPlay = "";
	float curFrameTime = 0.0;
	bool attackIsLeftHanded = false;
	bool isAtCorrectPositionInAnimation = false;

	cAnimationState* pAniState = pTheGO->pAnimationManager->pAniState;
	//TODO:: use this to draw the hand position
	glm::vec3 handPositionL(0.f);
	glm::vec3 handPositionR(0.f);

	//TODO:: find the time of the longest point of punch roughly 13/25 or half way
	//R3: B_R_Hand, Keyframe 10 / 25
	//L3 : B_L_Hand, Keyframe 13 / 25
	if (!pTheGO->pAnimationManager->animationsToRun.empty())
	{
		// Play the "1st" animation in the queue
		animationToPlay = pTheGO->pAnimationManager->animationsToRun[0].name;
		curFrameTime = pTheGO->pAnimationManager->animationsToRun[0].currentTime;

		float totalTime = pTheGO->pAnimationManager->animationsToRun[0].totalTime;

		float timePerFrame = totalTime / 25.f;

		//check to see what animation it is
		if (animationToPlay == "assets/modelsFBX/RPG-Character_Unarmed-Attack-R3(FBX2013).FBX") {
			attackIsLeftHanded = false;
			//check to see if the frame time is in the right area
			if (curFrameTime >= timePerFrame *17.4 && curFrameTime <= timePerFrame * 17.5) {
				isAtCorrectPositionInAnimation = true;
			}
			else {
				isAtCorrectPositionInAnimation = false;
			}
		}
		else if (animationToPlay == "assets/modelsFBX/RPG-Character_Unarmed-Attack-L3(FBX2013).FBX") {
			attackIsLeftHanded = true;
			//check to see if the frame time is in the right area
			if (curFrameTime >= timePerFrame * 21.4 && curFrameTime <= timePerFrame * 21.5f) {
				isAtCorrectPositionInAnimation = true;
			}
			else {
				isAtCorrectPositionInAnimation = false;
			}
		}

		// Increment the top animation in the queue
		if (pTheGO->pAnimationManager->animationsToRun[0].IncrementTime())
		{
			// The animation reset to zero on increment...
			// ...meaning that the 1st animation is done
			// (WHAT!? Should you use a vector for this???)
			pTheGO->pAnimationManager->animationsToRun.erase(pTheGO->pAnimationManager->animationsToRun.begin());
		}//vecAnimationQueue[0].IncrementTime()
	}
	else
	{	// Use the default animation.
		pAniState->defaultAnimation.IncrementTime();
		animationToPlay = pAniState->defaultAnimation.name;
		curFrameTime = pAniState->defaultAnimation.currentTime;
	}//if ( pAniState->vecAnimationQueue.empty()

	// Set up the animation pose:
	std::vector< glm::mat4x4 > vecFinalTransformation;	// Replaced by	theMesh.vecFinalTransformation
														//	std::vector< glm::mat4x4 > vecObjectBoneTransformation;
	std::vector< glm::mat4x4 > vecOffsets;
	// Final transformation is the bone transformation + boneOffsetPerVertex
	// ObjectBoneTransformation (or "Global") is the final location of the bones
	// vecOffsets is the relative offsets of the bones from each other

	//pass a vec 3 to this to get the position of the hand bone
	pTheGO->pAnimationManager->pSimpleSkinnedMesh->BoneTransform(
		curFrameTime,
		animationToPlay,		//**NEW**
		vecFinalTransformation,		// Final bone transforms for mesh
		theMesh.vecObjectBoneTransformation,  // final location of bones
		vecOffsets,
		handPositionL,
		handPositionR);                 // local offset for each bone

	unsigned int numberOfBonesUsed = static_cast<unsigned int>(vecFinalTransformation.size());
	glUniform1i(UniformLoc_numBonesUsed, numberOfBonesUsed);

	glm::mat4x4* pBoneMatrixArray = &(vecFinalTransformation[0]);

	// UniformLoc_bonesArray is the getUniformLoc of "bones[0]" from
	//	uniform mat4 bones[MAXNUMBEROFBONES]
	// in the shader
	glUniformMatrix4fv(UniformLoc_bonesArray, numberOfBonesUsed, GL_FALSE,
		(const GLfloat*)glm::value_ptr(*pBoneMatrixArray));

	// Update the extents of the skinned mesh from the bones...
	//	sMeshDrawInfo.minXYZ_from_SM_Bones(glm::vec3(0.0f)),
	//  sMeshDrawInfo.maxXYZ_from_SM_Bones(glm::vec3(0.0f))
	for (unsigned int boneIndex = 0; boneIndex != numberOfBonesUsed; boneIndex++)
	{
		glm::mat4 boneLocal;// = theMesh.vecObjectBoneTransformation[boneIndex];
		glm::mat4 modelMatrix(1.0f);
		boneLocal = pTheGO->pAnimationManager->pSimpleSkinnedMesh->mBoneInfo[boneIndex].ObjectBoneTransformation;
		//translate the matrix
		modelMatrix = glm::translate(modelMatrix, pTheGO->position);
		//rotate the matrix
		modelMatrix = glm::rotate(modelMatrix, pTheGO->orientation2.y, glm::vec3(0.f, 1.f, 0.f));
		//scale the matrix
		modelMatrix = glm::scale(modelMatrix, glm::vec3(pTheGO->scale));

		glm::vec4 boneBallLocation = modelMatrix * boneLocal * glm::vec4(0.f, 0.f, 0.f, 1.f);

		//TODO:: add a collision body to the bone
		//update the position of the body here

		// Update the extents of the mesh
		if (boneIndex == 0)
		{
			// For the 0th bone, just assume this is the extent
			theMesh.minXYZ_from_SM_Bones = glm::vec3(boneBallLocation);
			theMesh.maxXYZ_from_SM_Bones = glm::vec3(boneBallLocation);
		}
		else
		{	// It's NOT the 0th bone, so compare with current max and min
			if (theMesh.minXYZ_from_SM_Bones.x < boneBallLocation.x) { theMesh.minXYZ_from_SM_Bones.x = boneBallLocation.x; }
			if (theMesh.minXYZ_from_SM_Bones.y < boneBallLocation.y) { theMesh.minXYZ_from_SM_Bones.y = boneBallLocation.y; }
			if (theMesh.minXYZ_from_SM_Bones.z < boneBallLocation.z) { theMesh.minXYZ_from_SM_Bones.z = boneBallLocation.z; }

			if (theMesh.maxXYZ_from_SM_Bones.x > boneBallLocation.x) { theMesh.maxXYZ_from_SM_Bones.x = boneBallLocation.x; }
			if (theMesh.maxXYZ_from_SM_Bones.y > boneBallLocation.y)
			{
				theMesh.maxXYZ_from_SM_Bones.y = boneBallLocation.y;
			}
			if (theMesh.maxXYZ_from_SM_Bones.z > boneBallLocation.z)
			{
				theMesh.maxXYZ_from_SM_Bones.z = boneBallLocation.z;
			}
		}//if ( boneIndex == 0 )
	}

	//****************************************************************************************
	return;
}