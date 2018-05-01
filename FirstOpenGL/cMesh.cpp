#include "cMesh.h"

#include <glm/glm.hpp>		// cross product (I hope)

cMesh::cMesh(){
	this->numberOfVertices = 0;
	this->numberOfTriangles = 0;
	this->maxExtent = 0.0f;
	this->scaleForUnitBBox = 1.0f;
	return;
}

cMesh::~cMesh(){
	return;
}

void cMesh::CalculateExtents(void){
	// Assume 1st vertex is both max and min
	this->minXYZ.x = this->pVertices[0].x;
	this->minXYZ.y = this->pVertices[0].y;
	this->minXYZ.z = this->pVertices[0].z;
	this->maxXYZ.x = this->pVertices[0].x;
	this->maxXYZ.y = this->pVertices[0].y;
	this->maxXYZ.z = this->pVertices[0].z;

	for (int index = 0; index != this->numberOfVertices; index++){
		if (this->pVertices[index].x < this->minXYZ.x){
			this->minXYZ.x = this->pVertices[index].x;
			//get min x point
			this->minX.x = this->pVertices[index].x;
			this->minX.y = this->pVertices[index].y;
			this->minX.z = this->pVertices[index].z;
		}
		if (this->pVertices[index].x > this->maxXYZ.x){
			this->maxXYZ.x = this->pVertices[index].x;
			//get max x point
			this->maxX.x = this->pVertices[index].x;
			this->maxX.y = this->pVertices[index].y;
			this->maxX.z = this->pVertices[index].z;
		}
		// y...
		if (this->pVertices[index].y < this->minXYZ.y){
			this->minXYZ.y = this->pVertices[index].y;
			//get min y point
			this->minY.y = this->pVertices[index].y;
			this->minY.x = this->pVertices[index].x;
			this->minY.z = this->pVertices[index].z;
		}
		if (this->pVertices[index].y > this->maxXYZ.y){
			this->maxXYZ.y = this->pVertices[index].y;
			//get max y point

			//get min y point
			this->maxY.y = this->pVertices[index].y;
			this->maxY.x = this->pVertices[index].x;
			this->maxY.z = this->pVertices[index].z;
		}
		// z...
		if (this->pVertices[index].z < this->minXYZ.z){
			this->minXYZ.z = this->pVertices[index].z;

			//get min z point
			this->minZ.z = this->pVertices[index].z;
			this->minZ.x = this->pVertices[index].x;
			this->minZ.y = this->pVertices[index].y;
		}
		if (this->pVertices[index].z > this->maxXYZ.z){
			this->maxXYZ.z = this->pVertices[index].z;

			//get max z point
			this->maxZ.z = this->pVertices[index].z;
			this->maxZ.x = this->pVertices[index].x;
			this->maxZ.y = this->pVertices[index].y;
		}
	}//for ( int index...

	this->maxExtentXYZ.x = this->maxXYZ.x - this->minXYZ.x;
	this->maxExtentXYZ.y = this->maxXYZ.y - this->minXYZ.y;
	this->maxExtentXYZ.z = this->maxXYZ.z - this->minXYZ.z;

	this->maxExtent = this->maxExtentXYZ.x;
	if (this->maxExtent < this->maxExtentXYZ.y){
		// Y is bigger
		this->maxExtent = this->maxExtentXYZ.y;
	}
	if (this->maxExtent < this->maxExtentXYZ.z){
		// Z is bigger
		this->maxExtent = this->maxExtentXYZ.z;
	}
	//
	this->scaleForUnitBBox = 1.0f / this->maxExtent;

	return;
}

// Takes an indexed model and makes just a vertex array model
void cMesh::FlattenIndexedModel(void){
	// 1. Make a copy of the original vertices
	// 2. Using the triangle array, replace the original data

	int origNumVertices = this->numberOfVertices;

	//	cVertex_xyz_rgb vertOrig[MAX_VERTEX_ARRAY_SIZE]; // Stack

	sVertex_xyz_rgba_n_uv2_bt_4Bones* pVertOrig = new sVertex_xyz_rgba_n_uv2_bt_4Bones[this->numberOfVertices]; // Heap

	for (int index = 0; index < origNumVertices; index++){
		pVertOrig[index] = this->pVertices[index];
	}
	// We now have a copy of the original vertices

	// Adjust the original vertex array
	delete[] this->pVertices;		// GOTCHA!

//	int numberOfVertsTOTALGUESS = this->numberOfVertices * 4;
//	this->pVertices = new cVertex_xyz_rgb[numberOfVertsTOTALGUESS];

	// Actually, the number of vertices would be
	//	the number of triangles x 3...
	int numberOfVertsNeeded = this->numberOfTriangles * 3;
	numberOfVertsNeeded += 30;	// Add a few, just in case...
	this->pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[numberOfVertsNeeded];

	int triIndex = 0;
	int vertIndex = 0;
	for (; triIndex < this->numberOfTriangles;triIndex++, vertIndex += 3){
		// 3 4 132 80
		int triVert0_index = this->pTriangles[triIndex].vertex_ID_0;
		int triVert1_index = this->pTriangles[triIndex].vertex_ID_1;
		int triVert2_index = this->pTriangles[triIndex].vertex_ID_2;

		sVertex_xyz_rgba_n_uv2_bt_4Bones V0 = pVertOrig[triVert0_index];
		sVertex_xyz_rgba_n_uv2_bt_4Bones V1 = pVertOrig[triVert1_index];
		sVertex_xyz_rgba_n_uv2_bt_4Bones V2 = pVertOrig[triVert2_index];

		this->pVertices[vertIndex + 0] = V0;
		this->pVertices[vertIndex + 1] = V1;
		this->pVertices[vertIndex + 2] = V2;
	}//for ( ;

	// Update the number of vertices
	// HACK: -3 YIKES!!!
	this->numberOfVertices = vertIndex - 3;

	return;
}

// Used for the physics calculations
void cMesh::GeneratePhysicsTriangles(void){
	//std::vector< cPhysTriangle > vecPhysTris;

	for (int triIndex = 0; triIndex < this->numberOfTriangles; triIndex++){
		// 3 4 132 80
		int triVert0_index = this->pTriangles[triIndex].vertex_ID_0;
		int triVert1_index = this->pTriangles[triIndex].vertex_ID_1;
		int triVert2_index = this->pTriangles[triIndex].vertex_ID_2;

		cPhysTriangle tempTri;
		sVertex_xyz_rgba_n_uv2_bt_4Bones V0 = this->pVertices[triVert0_index];
		tempTri.vertex[0].x = V0.x;
		tempTri.vertex[0].y = V0.y;
		tempTri.vertex[0].z = V0.z;

		tempTri.normals[0].x = V0.nx;
		tempTri.normals[0].y = V0.ny;
		tempTri.normals[0].z = V0.nz;

		sVertex_xyz_rgba_n_uv2_bt_4Bones V1 = this->pVertices[triVert1_index];
		tempTri.vertex[1].x = V1.x;
		tempTri.vertex[1].y = V1.y;
		tempTri.vertex[1].z = V1.z;

		tempTri.normals[1].x = V1.nx;
		tempTri.normals[1].y = V1.ny;
		tempTri.normals[1].z = V1.nz;

		sVertex_xyz_rgba_n_uv2_bt_4Bones V2 = this->pVertices[triVert2_index];
		tempTri.vertex[2].x = V2.x;
		tempTri.vertex[2].y = V2.y;
		tempTri.vertex[2].z = V2.z;

		tempTri.normals[2].x = V2.nx;
		tempTri.normals[2].y = V2.ny;
		tempTri.normals[2].z = V2.nz;

		this->vecPhysTris.push_back(tempTri);
	}//for ( ;
	return;
}