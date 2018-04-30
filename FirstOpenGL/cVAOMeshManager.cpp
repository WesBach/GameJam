#include "cVAOMeshManager.h"

#include "globalOpenGL_GLFW.h"	// glad (gl.h) and GLFW
//#include <glad/glad.h>		// For all the OpenGL calls
//#include <GLFW/glfw3.h>		// For all the OpenGL calls


#include "cMesh.h"	// NOTE: This is in the CPP file, NOT the .h file

//// The vertex layout as defined by the shader
//// Where's the Best Place to put this?? 
//// (good question)
//class cVertex
//{
//public:
//    float x, y, z;		// Position (vec2)	float x, y;	
//    float r, g, b;		// Colour (vec3)
//	float nx, ny, nz;	// Now with normals!
//};
#include "sVertex_xyz_rgba_n_uv2_bt.h"			

cVAOMeshManager::cVAOMeshManager()
{

	return;
}

cVAOMeshManager::~cVAOMeshManager()
{

	return;
}

bool cVAOMeshManager::loadMeshIntoVAO(cMesh &theMesh, int shaderID, bool bKeepMesh /*=false*/)
{
	// ******************************************************************************
	//__      __       _              ____         __  __          
	//\ \    / /      | |            |  _ \       / _|/ _|         
	// \ \  / /__ _ __| |_ _____  __ | |_) |_   _| |_| |_ ___ _ __ 
	//  \ \/ / _ \ '__| __/ _ \ \/ / |  _ <| | | |  _|  _/ _ \ '__|
	//   \  /  __/ |  | ||  __/>  <  | |_) | |_| | | | ||  __/ |   
	//    \/ \___|_|   \__\___/_/\_\ |____/ \__,_|_| |_| \___|_|   
	//                                                             
	// ******************************************************************************

	sVAOInfo theVAOInfo;

	// Create a Vertex Array Object (VAO)
	glGenVertexArrays(1, &(theVAOInfo.VAO_ID));
	glBindVertexArray(theVAOInfo.VAO_ID);


	glGenBuffers(1, &(theVAOInfo.vertex_buffer_ID));
	glBindBuffer(GL_ARRAY_BUFFER, theVAOInfo.vertex_buffer_ID);

	// Allocate the global vertex array
	//cVertex* pVertices = new cVertex[theMesh.numberOfVertices];
	sVertex_xyz_rgba_n_uv2_bt_4Bones* pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[theMesh.numberOfVertices];

	for (int index = 0; index < theMesh.numberOfVertices; index++)
	{
		pVertices[index].x = theMesh.pVertices[index].x;
		pVertices[index].y = theMesh.pVertices[index].y;
		pVertices[index].z = theMesh.pVertices[index].z;

		pVertices[index].r = theMesh.pVertices[index].r;
		pVertices[index].g = theMesh.pVertices[index].g;
		pVertices[index].b = theMesh.pVertices[index].b;
		pVertices[index].a = theMesh.pVertices[index].a;

		pVertices[index].nx = theMesh.pVertices[index].nx;
		pVertices[index].ny = theMesh.pVertices[index].ny;
		pVertices[index].nz = theMesh.pVertices[index].nz;

		// Other additions: texture coords, tangent, and bi-normal
		pVertices[index].u1 = theMesh.pVertices[index].u1;
		pVertices[index].v1 = theMesh.pVertices[index].v1;
		pVertices[index].u2 = theMesh.pVertices[index].u2;
		pVertices[index].v2 = theMesh.pVertices[index].v2;

		pVertices[index].bx = theMesh.pVertices[index].bx;
		pVertices[index].by = theMesh.pVertices[index].by;
		pVertices[index].bz = theMesh.pVertices[index].bz;

		pVertices[index].tx = theMesh.pVertices[index].tx;
		pVertices[index].ty = theMesh.pVertices[index].ty;
		pVertices[index].tz = theMesh.pVertices[index].tz;

		pVertices[index].boneID[0] = theMesh.pVertices[index].boneID[0];
		pVertices[index].boneID[1] = theMesh.pVertices[index].boneID[1];
		pVertices[index].boneID[2] = theMesh.pVertices[index].boneID[2];
		pVertices[index].boneID[3] = theMesh.pVertices[index].boneID[3];

		pVertices[index].boneWeights[0] = theMesh.pVertices[index].boneWeights[0];
		pVertices[index].boneWeights[1] = theMesh.pVertices[index].boneWeights[1];
		pVertices[index].boneWeights[2] = theMesh.pVertices[index].boneWeights[2];
		pVertices[index].boneWeights[3] = theMesh.pVertices[index].boneWeights[3];
		// Or, since these are the same, you could simply assign one to the other...
		//		pVertices[index] = theMesh.pVertices[index];
	}

	// Copy the local vertex array into the GPUs memory
	//int sizeOfGlobalVertexArrayInBytes = sizeof(cVertex) * theMesh.numberOfVertices;
	int sizeOfGlobalVertexArrayInBytes = sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones) * theMesh.numberOfVertices;
	glBufferData(GL_ARRAY_BUFFER,
		sizeOfGlobalVertexArrayInBytes,		// sizeof(vertices), 
		pVertices,
		GL_STATIC_DRAW);

	// Get rid of local vertex array
	delete[] pVertices;
	// ******************************************************************************
	// ******************************************************************************

	glGenBuffers(1, &(theVAOInfo.index_buffer_ID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theVAOInfo.index_buffer_ID); // It's now an Index Buffer

																	   // Make a temporary array that matches what will be on the GPU...
																	   // (GPU expects a 1D array of integers)

																	   // Number of indices = number of tris x 3 (3 indices per triangle)
	int numberOfIndices = theMesh.numberOfTriangles * 3;

	//unsigned int indexArray[ 10000000 ];		// CAN'T DO THIS, unfortunately
	unsigned int* indexArray = new unsigned int[numberOfIndices];

	// Copy the triangle data into this 1D array 
	int triIndex = 0;		// Index into the triangle array (from mesh)
	int indexIndex = 0;		// Index into the "vertex index" array (1D)
	for (; triIndex < theMesh.numberOfTriangles; triIndex++, indexIndex += 3)
	{
		indexArray[indexIndex + 0] = theMesh.pTriangles[triIndex].vertex_ID_0;
		indexArray[indexIndex + 1] = theMesh.pTriangles[triIndex].vertex_ID_1;
		indexArray[indexIndex + 2] = theMesh.pTriangles[triIndex].vertex_ID_2;
	}
	// note number of indices is number of triangles x 3
	int sizeOfIndexArrayInBytes = sizeof(unsigned int) * numberOfIndices;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,		// "index buffer" or "vertex index buffer"
		sizeOfIndexArrayInBytes,
		indexArray,
		GL_STATIC_DRAW);

	// Don't need local array anymore (like the on in CPU RAM)
	delete[] indexArray;

	// At this point, the vertex and index buffers are stored on GPU

	// Now set up the vertex layout (for this shader)

	GLuint vpos_location = glGetAttribLocation(shaderID, "vPos");		// program, "vPos");	// 6
	GLuint vcol_location = glGetAttribLocation(shaderID, "vCol");		// program, "vCol");	// 13
	GLuint vnorm_location = glGetAttribLocation(shaderID, "vNorm");		// program, "vCol");	// 13
																		// ADDED: December 4
	GLuint vUVx2_location = glGetAttribLocation(shaderID, "uvX2");		// program, "vCol");	// 13

	GLuint vTangent_location = glGetAttribLocation(shaderID, "vTangent");				// in vec3 vTangent;
	GLuint vBitangent_location = glGetAttribLocation(shaderID, "vBitangent");			// in vec3 vBitangent					
	GLuint vBoneWeights_x4_location = glGetAttribLocation(shaderID, "vBoneWeights_x4");	// 	in vec4 vBoneWeights_x4;
	GLuint vBoneIDs_x4_location = glGetAttribLocation(shaderID, "vBoneIDs_x4");			// in vec4 vBoneIDs_x4;		


																		// Size of the vertex we are using in the array.
																		// This is called the "stride" of the vertices in the vertex buffer
	const unsigned int VERTEX_SIZE_OR_STRIDE_IN_BYTES = sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones);

	{//STARTOF: "in vec3 vPos;" or "float x, y, z;"
		glEnableVertexAttribArray(vpos_location);
		const unsigned int OFFSET_TO_X_IN_CVERTEX = offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, x);
		glVertexAttribPointer(vpos_location,
			3,				// now vec3, not vec2   
			GL_FLOAT,
			GL_FALSE,
			VERTEX_SIZE_OR_STRIDE_IN_BYTES,		//sizeof(float) * 9,	// cVertex is 6 floats in size
												// or you could do: sizeof( cVertex ), 
												//	  (void*) (sizeof(float) * offsetOf_x_into_cVertex) );		
												//	  (void*) offsetof( cVertex, x ) );
			reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X_IN_CVERTEX)));	// 64-bit
	}//ENDOF: "in vec3 vPos;" or "float x, y, z;"

	{//STARTOF: "in vec3 vPos;" or "float x, y, z;"
		glEnableVertexAttribArray(vpos_location);
		const unsigned int OFFSET_TO_X_IN_CVERTEX = offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, x);
		glVertexAttribPointer(vpos_location,
			3,				// now vec3, not vec2   
			GL_FLOAT,
			GL_FALSE,
			VERTEX_SIZE_OR_STRIDE_IN_BYTES,		//sizeof(float) * 9,	// cVertex is 6 floats in size
												// or you could do: sizeof( cVertex ), 
												//	  (void*) (sizeof(float) * offsetOf_x_into_cVertex) );		
												//	  (void*) offsetof( cVertex, x ) );
			reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X_IN_CVERTEX)));	// 64-bit
	}//ENDOF: "in vec3 vPos;" or "float x, y, z;"

	{//STARTOF: "in vec4 vCol;" or "float r, g, b, a;"
		glEnableVertexAttribArray(vcol_location);
		const unsigned int OFFSET_TO_R_IN_CVERTEX = offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, r);
		glVertexAttribPointer(vcol_location,
			4,				// Was 3, but now has alpha
			GL_FLOAT,
			GL_FALSE,
			VERTEX_SIZE_OR_STRIDE_IN_BYTES,		// sizeof(float) * 9,		// (  sizeof(cVertex)  );
			reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_R_IN_CVERTEX)));	// 64-bit
	}// ENDOF: "in vec4 vCol;" or "float r, g, b, a;"

	{//STARTOF: "in vec3 vNorm;" or "float nx, ny, nz;"
		glEnableVertexAttribArray(vnorm_location);
		const unsigned int OFFSET_TO_NX_IN_CVERTEX = offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, nx);
		glVertexAttribPointer(vnorm_location,
			3,
			GL_FLOAT,
			GL_FALSE,
			VERTEX_SIZE_OR_STRIDE_IN_BYTES,
			reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_NX_IN_CVERTEX)));	// 64-bit
	}//ENDOF: "in vec3 vNorm;" or "float nx, ny, nz;"

	{//STARTOF: "in vec4 uvX2;" or "float u1, v1, u2, v2;"
		glEnableVertexAttribArray(vUVx2_location);
		const unsigned int OFFSET_TO_UVs_IN_CVERTEX = offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, u1);
		glVertexAttribPointer(vUVx2_location,
			4,				// because "vec4"
			GL_FLOAT,			// vec is a float 
			GL_FALSE,			// normalize or not
			VERTEX_SIZE_OR_STRIDE_IN_BYTES,
			reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_UVs_IN_CVERTEX)));	// 64-bit
	}//ENDOF: "in vec4 uvX2;" or "float u1, v1, u2, v2;"
	 // *******************************************************************
	{//STARTOF: "in vec3 vTangent;" or "float tx, ty, tz;"
		glEnableVertexAttribArray(vTangent_location);
		const unsigned int OFFSET_TO_vTangent_IN_CVERTEX = offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, tx);
		glVertexAttribPointer(vTangent_location,
			3,				// because "vec3"
			GL_FLOAT,			// vec is a float 
			GL_FALSE,			// normalize or not
			VERTEX_SIZE_OR_STRIDE_IN_BYTES,
			reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vTangent_IN_CVERTEX)));	// 64-bit
	}//ENDOF: "in vec3 vTangent;" or "float tx, ty, tz;"

	{//STARTOF: "in vec3 vBitangent;" or "float bx, by, bz;"
	 //GLuint vBitangent_location = glGetAttribLocation(shaderID, "vBitangent");			// in vec3 vBitangent;
		glEnableVertexAttribArray(vBitangent_location);
		const unsigned int OFFSET_TO_vBitangent_IN_CVERTEX = offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, bx);
		glVertexAttribPointer(vBitangent_location,
			3,				// because "vec3"
			GL_FLOAT,			// vec is a float 
			GL_FALSE,			// normalize or not
			VERTEX_SIZE_OR_STRIDE_IN_BYTES,
			reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBitangent_IN_CVERTEX)));	// 64-bit
	}//ENDOF: "in vec3 vBitangent;" or "float bx, by, bz;"

	{//STARTOF: "in vec4 vBoneIDs_x4;" or "float boneID[NUMBEROFBONES];"
	 //GLuint vBoneIDs_x4_location = glGetAttribLocation(shaderID, "vBoneIDs_x4");			// in vec4 vBoneIDs_x4;		
		glEnableVertexAttribArray(vBoneIDs_x4_location);
		const unsigned int OFFSET_TO_vBoneIDs_x4_IN_CVERTEX
			= (unsigned int)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, boneID[0]);
		glVertexAttribPointer(vBoneIDs_x4_location,
			4,				// because "vec4"
			GL_FLOAT,			// vec is a float 
			GL_FALSE,			// normalize or not
			VERTEX_SIZE_OR_STRIDE_IN_BYTES,
			reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBoneIDs_x4_IN_CVERTEX)));	// 64-bit
	}//ENDOF: "in vec4 vBoneIDs_x4;" or "float boneID[NUMBEROFBONES];"

	{//STARTOF: "in vec4 vBoneWeights_x4;" or "float boneWeights[NUMBEROFBONES];"
	 //GLuint vBoneWeights_x4_location = glGetAttribLocation(shaderID, "vBoneWeights_x4");	// 	in vec4 vBoneWeights_x4;	
		glEnableVertexAttribArray(vBoneWeights_x4_location);
		const unsigned int OFFSET_TO_vBoneWeights_x4_IN_CVERTEX
			= (unsigned int)offsetof(sVertex_xyz_rgba_n_uv2_bt_4Bones, boneWeights[0]);
		glVertexAttribPointer(vBoneWeights_x4_location,
			4,				// because "vec4"
			GL_FLOAT,			// vec is a float 
			GL_FALSE,			// normalize or not
			VERTEX_SIZE_OR_STRIDE_IN_BYTES,
			reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_vBoneWeights_x4_IN_CVERTEX)));	// 64-bit
	}//ENDOF: "in vec4 vBoneWeights_x4;" or "float boneWeights[NUMBEROFBONES];"

																					
	theVAOInfo.numberOfIndices = theMesh.numberOfTriangles * 3;
	theVAOInfo.numberOfTriangles = theMesh.numberOfTriangles;
	theVAOInfo.numberOfVertices = theMesh.numberOfVertices;
	theVAOInfo.friendlyName = theMesh.name;
	theVAOInfo.shaderID = shaderID;

	theMesh.CalculateExtents();
	theVAOInfo.scaleForUnitBBox = theMesh.scaleForUnitBBox;

	// Store the VAO info by mesh name
	this->m_mapNameToVAO[theMesh.name] = theVAOInfo;

	// CRITICAL 
	// Unbind the VAO first!!!!
	glBindVertexArray(0);	// 

	// Unbind (release) everything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vnorm_location);
	glDisableVertexAttribArray(vUVx2_location);
	glDisableVertexAttribArray(vTangent_location);				// in vec3 vTangent;
	glDisableVertexAttribArray(vBitangent_location);			// in vec3 vBitangent;
	glDisableVertexAttribArray(vBoneIDs_x4_location);			// in vec4 vBoneIDs_x4;		
	glDisableVertexAttribArray(vBoneWeights_x4_location);		// 	in vec4 vBoneWeights_x4;	
	// Save this mesh? 
	if (bKeepMesh)
	{	// Make a COPY for later...
		//std::map< std::string, cMesh > m_mapNameToMesh;
		this->m_mapNameToMesh[theMesh.name] = theMesh;
	}//if (bKeepMesh)

	return true;
}

bool cVAOMeshManager::lookupMeshFromName(std::string name, cMesh &theMesh)
{
	// Search for mesh by name using iterator based find
	std::map< std::string, cMesh >::iterator itMesh = this->m_mapNameToMesh.find(name);

	// Find it? 
	if (itMesh == this->m_mapNameToMesh.end())
	{	// Nope. 
		return false;
	}
	// Found the mesh (DIDN'T return .end())
	theMesh = itMesh->second;		// "return" the mesh by reference 
	return true;
}

bool cVAOMeshManager::lookupVAOFromName(std::string name, sVAOInfo &theVAOInfo)
{
	// look up in map for the name of the mesh we want to draw
	//	std::map< std::string, sVAOInfo > m_mapNameToVAO;
	// "Interator" is a class that can access inside a container
	std::map< std::string, sVAOInfo >::iterator itVAO = this->m_mapNameToVAO.find(name);
	// Did I find something?
	if (itVAO == this->m_mapNameToVAO.end())
	{	// ON NO! we DIDN'T!
		return false;
	}
	// DID find what we were looking for, so 
	//	ISN'T pointing to the "end()" built-in iterator
	theVAOInfo = itVAO->second;		// Because the "second" thing is the sVAO
	return true;
}