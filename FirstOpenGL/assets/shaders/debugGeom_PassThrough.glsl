// Debug GEOMETRY shader
#version 400 core

layout(triangles) 			in;
layout(triangle_strip) 		out;
layout(max_vertices = 3) 	out;

// gl_in[] array is built in (don't have to declare)
// gl_in[0] is the 1st vertex
// gl_in[1] is the 2nd vertex
// gl_in[2] is the 3rd vertex

in vec4 gVertColour[];		// in from the vertex shader
out vec4 fVertColour;		// out to the fragment shader

void main()
{
	vec3 triCentre = ( gl_in[0].gl_Position.xyz + 
	                   gl_in[1].gl_Position.xyz + 
					   gl_in[2].gl_Position.xyz ) / 3.0f;
	
	vec4 red = 	 vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec4 green = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vec4 blue =  vec4(0.0f, 0.0f, 1.0f, 1.0f);
	
	// 0, 1, centre
    gl_Position = vec4(triCentre, 1.0f);	fVertColour = red;
    //gl_Position = gl_in[2].gl_Position;		fVertColour = red;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position;		fVertColour = green;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;		fVertColour = blue;
    EmitVertex();
    EndPrimitive();

	// centre, 1, 2
   gl_Position = vec4(triCentre, 1.0f);	fVertColour = green;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;		fVertColour = green;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;		fVertColour = green;
    EmitVertex();
   EndPrimitive();

	// 0, centre, 2
    gl_Position = gl_in[0].gl_Position;		fVertColour = blue;
    EmitVertex();
    gl_Position = vec4(triCentre, 1.0f);	fVertColour = blue;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;		fVertColour = blue;
    EmitVertex();
    EndPrimitive();

 
}


//void main()
//{
//    gl_Position = gl_in[0].gl_Position;
//	//fVertColour = gVertColour[0];
//	fVertColour = vec4(1.0f, 0.0f, 0.0f, 1.0f);
//    EmitVertex();
//
//    gl_Position = gl_in[1].gl_Position;
//	//fVertColour = gVertColour[1];
// 	fVertColour = vec4(0.0f, 1.0f, 0.0f, 1.0f);
 //   EmitVertex();
//	
 //   gl_Position = gl_in[2].gl_Position;
//	fVertColour = gVertColour[2];
 //	fVertColour = vec4(0.0f, 0.0f, 1.0f, 1.0f);
 //   EmitVertex();
//
  //  EndPrimitive();
//}



