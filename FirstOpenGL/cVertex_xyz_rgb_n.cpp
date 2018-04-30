#include "cVertex_xyz_rgb_n.h"

float x = 0.0f;

// Constructor
cVertex_xyz_rgb_n::cVertex_xyz_rgb_n()	
{
	this->x = 0.0f; 
	this->y = 0.0f;
	this->z = 0.0f;
	this->r = 0.0f;
	this->g = 0.0f;
	this->b = 0.0f;
	this->nx = 0.0f;
	this->ny = 0.0f;
	this->nz = 0.0f;
	return;
}

	// Destructor
cVertex_xyz_rgb_n::~cVertex_xyz_rgb_n()
{
	// Doesn't really do anything
	return;
}
