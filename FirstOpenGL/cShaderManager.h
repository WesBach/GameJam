#ifndef _cShaderManager_HG_
#define _cShaderManager_HG_


#include <string>
#include <vector>
#include <map>
#include "sTextureBindBlendInfo.h"

class CTextureManager;
class cShader;
class cShaderProgram;

class cShaderManager
{
public:
	cShaderManager();
	~cShaderManager();

	bool useShaderProgram(unsigned int ID);
	bool useShaderProgram(std::string friendlyName);
	bool createProgramFromFile(std::string friendlyName,
		cShader &vertexShad,
		cShader &fragShader);
	bool createProgramFromSource(std::string friendlyName,
		cShader &vertexShad,
		cShader &fragShader);
	void setBasePath(std::string basepath);
	unsigned int getIDFromFriendlyName(std::string friendlyName);

	// Returns NULL if not found
	cShaderProgram* getShaderProgPointerFromID(unsigned int shaderID);
	cShaderProgram* getShaderProgPointerFromFriendlyName(std::string friendlyName);

	// Clears last error
	std::string getLastError(void);
private:
	// Returns an empty string if it didn't work
	bool m_loadSourceFromFile(cShader &shader);
	std::string m_basepath;

	bool m_compileShaderFromSource(cShader &shader, std::string &error);
	// returns false if no error
	bool m_wasThereACompileError(unsigned int shaderID, std::string &errorText);
	bool m_wasThereALinkError(unsigned int progID, std::string &errorText);

	std::string m_lastError;

	std::map< unsigned int /*ID*/, cShaderProgram > m_ID_to_Shader;
	std::map< std::string /*name*/, unsigned int /*ID*/ > m_name_to_ID;
};

#endif