#include "cTextRenderer.h"

#include "cShaderProgramInfo.h"
#include "cShaderManager.h"

const char* mvs_text = R"glsl(
#version 410
attribute vec4 coord;
varying vec2 texpos;
void main () {
	gl_Position = vec4(coord.xy, 0, 1);
	texpos = coord.zw;
})glsl";

const char* mfs_text = R"glsl(
#version 410
varying vec2 texpos;
uniform bool igcolour;
uniform sampler2D tex;
uniform vec4 color;
void main () {
	vec4 sampleColour = texture2D(tex, texpos);
	if(igcolour){
		gl_FragColor = vec4(0.f, 0.f, 1.f, 1.f);
	} else if (sampleColour.r < 0.1) {
		discard;
	}else{
		gl_FragColor = vec4(1, 1, 1, sampleColour.r) * color;
	}
})glsl";


cTextRenderer::cTextRenderer() {
	this->mfontSize = 16;
	//set the shader text
	this->m_fragmentShaderSource = mfs_text;
	this->m_vertexShaderSource = mvs_text;

	this->m_pShaderProg = new cShaderProgramInfo();
	
	//initialize freetype
	if (FT_Init_FreeType(&this->mft))
	{
		fprintf(stderr, "unable to init free type\n");
	}

	if (FT_New_Face(this->mft, "assets\\fonts\\FreeSans.ttf", 0, &this->mface))
	{
		fprintf(stderr, "unable to open font\n");
	}

	//set font size
	FT_Set_Pixel_Sizes(this->mface, 0, this->mfontSize);

	if (FT_Load_Char(this->mface, 'X', FT_LOAD_RENDER))
	{
		fprintf(stderr, "unable to load character\n");
	}
}

cTextRenderer::~cTextRenderer()
{
	if (this->m_pShaderProg)
	{
		delete this->m_pShaderProg;
	}
}

GLint cTextRenderer::getFontSize() {
	return this->mfontSize;
}

void cTextRenderer::setFontSize(const int& fontSize) {
	this->mfontSize = fontSize;
}

void cTextRenderer::render_text(const wchar_t *text, float x, float y, float sx, float sy) {

	// Get the uniform variable locations 
	glUseProgram(this->m_pShaderProg->shaderProgramID);

	//generate and bind vbo 
	glGenBuffers(1, &mdp_vbo);
	//generate and bind vao
	glGenVertexArrays(1, &mvao);

	//color
	GLfloat yellow[4] = { 1, 1, 0, 1 };
	//set the color and bind the vertex array
	glUniform4fv(uniform_color, 1, yellow);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE); 

	//bind vert buffer
	glBindVertexArray(mvao);

	glUniform1i(uniform_igcolour, false);
	const wchar_t *p;
	FT_GlyphSlot g = mface->glyph;

	//textures
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(uniform_tex, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, mdp_vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);


	/* Draw the background on the screen */
	//point back[4] = {
	//{ 0, 0, 0, 0 },
	//{ 0, (float)1/200, 1, 0 },
	//{ (float)1/200, 0, 0, 1 },
	//{ (float)1/200, (float)1/200, 1, 1 },
	//};
	//glBufferData(GL_ARRAY_BUFFER, sizeof(back), back, GL_DYNAMIC_DRAW);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glUniform1i(uniform_igcolour, false);

	/* Loop through all characters */
	for (p = text; *p; p++) {
		/* Try to load and render the character */
		if (FT_Load_Char(mface, *p, FT_LOAD_RENDER))
			continue;

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		point box[4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	//clean up
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glDisableVertexAttribArray(attribute_coord);
	glDeleteTextures(1, &tex);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

bool cTextRenderer::initializeTextRenderer(std::string& error) {
	cShaderManager shaderManager;

	//parse the vertex and fragment shader source
	this->m_pShaderProg->vertShader->parseStringIntoMultiLine(this->m_vertexShaderSource);
	this->m_pShaderProg->fragShader->parseStringIntoMultiLine(this->m_fragmentShaderSource);

	//create the program
	if (!shaderManager.createProgramFromSource("textShader",
		*this->m_pShaderProg->vertShader,
		*this->m_pShaderProg->fragShader))
	{
		error = "Could not create the debug shader program.\n" + shaderManager.getLastError();
		return false;
	}
	// The shader was compiled, so get the shader program number
	this->m_pShaderProg->shaderProgramID = shaderManager.getIDFromFriendlyName("textShader");

	glUseProgram(this->m_pShaderProg->shaderProgramID);

	//get vertex attribute/s id/s
	attribute_coord = glGetAttribLocation(this->m_pShaderProg->shaderProgramID, "coord");
	uniform_igcolour = glGetUniformLocation(this->m_pShaderProg->shaderProgramID, "igcolour");
	uniform_tex = glGetUniformLocation(this->m_pShaderProg->shaderProgramID, "tex");
	uniform_color = glGetUniformLocation(this->m_pShaderProg->shaderProgramID, "color");

	if (attribute_coord == -1 || uniform_tex == -1 || uniform_color == -1 || uniform_igcolour == -1) {
		fprintf(stderr, "unable to get attribute or uniform from shader\n");
		return GL_FALSE;
	}

	return GL_TRUE;
}
