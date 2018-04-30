#include "cMenuRenderer.h"
#include "cShaderProgramInfo.h"
#include "cShaderManager.h"
#include <iostream>

const char* menuVertexShader_text = R"glsl(
#version 410
attribute vec4 coord;
varying vec2 texpos;
void main () {
	gl_Position = vec4(coord.xy, 0, 1);
	texpos = coord.zw;
})glsl";

const char* menuFragmentShader_text = R"glsl(
#version 410
varying vec2 texpos;
uniform bool igcolour;
uniform sampler2D tex;
uniform vec4 color;
void main () {
	vec4 sampleColour = texture2D(tex, texpos);
	if(igcolour){
		gl_FragColor = vec4(0.f, 0.f, 1.f, 1.f);
	} 
	else{
		gl_FragColor = vec4(1, 1, 1, sampleColour.r) * color;
	}
})glsl";


cMenuRenderer::cMenuRenderer() {
	std::string error;
	//set the shader text
	this->m_fragmentShaderSource = menuFragmentShader_text;
	this->m_vertexShaderSource = menuVertexShader_text;

	this->m_pShaderProg = new cShaderProgramInfo();

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
		std::cout << error << std::endl;
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
		std::cout << "unable to get attribute or uniform from shader" << std::endl;
	}

}


bool cMenuRenderer::addMenuItem(sMenuItem* item) {
	//check to see if the item exists already 
	for (int i = 0; i < this->vecMenuItems.size(); i++)
	{
		if (this->vecMenuItems[i] == item)
		{
			return false;
		}
	}
	//no in the vector so add it 
	this->vecMenuItems.push_back(item);

	return true;
}

bool cMenuRenderer::removeMenuItem(sMenuItem* item) {
	for (std::vector<sMenuItem*>::iterator it = this->vecMenuItems.begin(); it != this->vecMenuItems.end(); it++)
	{
		if (*it == item)
		{
			it = this->vecMenuItems.erase(it);
			return true;
		}
	}

	return false;
}

bool cMenuRenderer::renderMenu() {
	// Get the uniform variable locations 
	glUseProgram(this->m_pShaderProg->shaderProgramID);

	//generate and bind vbo 
	glGenBuffers(1, &mdp_vbo);
	//generate and bind vao
	glGenVertexArrays(1, &mvao);

	//draw the menu items
	for (int i = 0; i < this->vecMenuItems.size(); i++)
	{
		drawMenuItem(this->vecMenuItems[i]);
	}

	//clean up
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(attribute_coord);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	return true;
}

bool cMenuRenderer::drawMenuItem(sMenuItem* item) {
	//color
	GLfloat yellow[4] = { 1, 1, 0, 1 };
	//set the color and bind the vertex array
	glUniform4fv(uniform_color, 1, yellow);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);

	//bind vert buffer
	glBindVertexArray(mvao);

	glUniform1i(uniform_igcolour, false);

	//TODO:: make the quad colors match a texture

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
	//change this to be the size of the menu items

	//TODO:: map the point coords from the item width and height
	point back[4] = {
	{ 0, 0, 0, 0 },
	{ 0, 1/item->height, 1 / item->width, 0 },
	{ 1 / item->width, 0, 0, 1 / item->height },
	{ 1 / item->width, 1 / item->height, 1 / item->width, 1 / item->height },
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(back), back, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDeleteTextures(1, &tex);

	return true;
}

