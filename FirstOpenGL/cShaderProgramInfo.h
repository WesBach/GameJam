#ifndef _cShaderProgramInfo_HG_
#define _cShaderProgramInfo_HG_
#include "cShader.h"

class cShaderProgramInfo
{
public:
	cShaderProgramInfo() :
		shaderProgramID(0),
		matProjectionUniformLoc(-1),
		matViewUniformLoc(-1),
		matModelUniformLoc(-1)
	{
		this->fragShader = new cShader();
		this->vertShader = new cShader();
	};

	~cShaderProgramInfo() { 
		delete this->vertShader;
		delete this->fragShader;
	}

	cShader* vertShader;
	cShader* fragShader;
	unsigned int shaderProgramID;
	// Uniforms in the shader
	int matProjectionUniformLoc;
	int matViewUniformLoc;
	int matModelUniformLoc;
};
#endif // !_cShaderProgramInfo_HG_

