#version 420

in vec4 vertColour;	

void main()
{	
	gl_FragColor.rgb = vertColour.rgb;
	gl_FragColor.a = vertColour.a;		
}
