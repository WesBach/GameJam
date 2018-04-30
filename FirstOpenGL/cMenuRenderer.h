#ifndef  _cMenuRenderer
#define _cMenuRenderer_HG_
#include <glad\glad.h>
#include <glm\vec3.hpp>
#include "sMenuItem.h"

class cShaderProgramInfo;

class cMenuRenderer {
public :

	struct point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	};

	cMenuRenderer();

	bool addMenuItem(sMenuItem* item);
	bool removeMenuItem(sMenuItem* item);

	bool renderMenu();

private:
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

	//vector of menu items to draw
	std::vector<sMenuItem*> vecMenuItems;

	bool drawMenuItem(sMenuItem* item);
};

#endif // ! _cMenuRenderer

