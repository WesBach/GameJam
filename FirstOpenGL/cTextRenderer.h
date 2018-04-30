#ifndef _cTextRenderer_HG_
#define _cTextRenderer_HG_

#include "pugixml.hpp"
#include <sapi.h>
#include <sphelper.h>
#include "stb_image.h" 
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad\glad.h>

class cShaderProgramInfo;

class cTextRenderer {
public:
	cTextRenderer();
	~cTextRenderer();
	struct point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	};

	void render_text(const wchar_t *text, float x, float y, float sx, float sy);
	GLint getFontSize();
	void setFontSize(const int& fontSize);
	bool initializeTextRenderer(std::string& error);

private:
	//freetypes
	FT_Library mft;
	FT_Face mface;

	GLint mfontSize;

	GLint attribute_coord;
	GLint uniform_igcolour;
	GLint uniform_tex;
	GLint uniform_color;

	//vertex array object
	GLuint mvao;
	//vertex buffer object
	GLuint mdp_vbo;
	
	std::string m_vertexShaderSource;
	std::string m_fragmentShaderSource;
	//shader prog info
	cShaderProgramInfo* m_pShaderProg;
};

#endif // !_cTextRenderer_HG_

