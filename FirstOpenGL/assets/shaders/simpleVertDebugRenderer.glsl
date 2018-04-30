#version 420

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;

attribute vec4 vPosition;			
attribute vec4 vColour;	

out vec4 vertColour;		

void main()
{
	mat4 MVP = mProjection * mView * mModel;
	gl_Position = MVP * vec4(vPosition, 1.0f);
	
    vertColour = vColour;
}

