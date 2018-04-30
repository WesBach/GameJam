#version 400
out vec4 FragColor;
out vec4 BrightColor;

uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;  // rgb = colour of HIGHLIGHT only
uniform vec3 eyePosition;	// Camera position

struct sFBOout
{
	vec4 FragPos;
	vec4 Normal;
	vec4 TexCoords;
	vec4 WorldPostition;
};
in sFBOout fragOut;

struct Light {
    vec3 Position;
    vec3 Color;
};

uniform Light lights[4];
uniform sampler2D diffuseTexture;
uniform vec3 viewPos;

struct sLightDesc {
	vec4 position;
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;		// Colour (xyz), intensity (w)
	vec4 attenuation;	// x = constant, y = linear, z = quadratic
	vec4 direction;
	vec4 typeParams;	// x = type
						// 		0 = point
						// 		1 = directional
						// 		2 = spot
						// y = distance cut-off
						// z angle1, w = angle2		- only for spot
};

const int NUMBEROFLIGHTS = 20;
uniform sLightDesc myLight[NUMBEROFLIGHTS];
// This function has now been moved to the bottom of the file...
// Calculate the contribution of a light at a vertex
vec3 calcLightColour(in vec3 vecNormal,
	in vec3 vecWorldPosition,
	in int lightID,
	in vec3 matDiffuse,
	in vec4 matSpecular);
/*****************************************************/

void main()
{           
    vec3 color = texture(diffuseTexture, fragOut.TexCoords.xy).rgb;
    vec3 normal = normalize(fragOut.Normal.xyz);
    // ambient
    vec3 ambient = 0.0 * color;
	
	
    // // lighting
     vec3 lighting = vec3(0.0);
    // vec3 viewDir = normalize(viewPos - sFBOout.FragPos);
    // for(int i = 0; i < 4; i++)
    // {
        // // diffuse
        // vec3 lightDir = normalize(lights[i].Position - sFBOout.FragPos);
        // float diff = max(dot(lightDir, normal), 0.0);
        // vec3 result = lights[i].Color * diff * color;      
        // // attenuation (use quadratic as we have gamma correction)
        // float distance = length(sFBOout.FragPos - lights[i].Position);
        // result *= 1.0 / (distance * distance);
        // lighting += result;
                
    // }
	
	//calc lighting
			// ****************************************************************/	
		// NOW, in the 2nd pass, we do the lighting!!! 
	for (int index = 0; index < NUMBEROFLIGHTS; index++)
	{
		lighting.rgb += calcLightColour(fragOut.Normal.xyz,
			fragOut.WorldPostition.xyz,
			index,
			color,
			materialSpecular);
	}
	
	
    vec3 result = ambient + lighting;
    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    FragColor = vec4(result, 1.0);
}


// Inspired by Mike Bailey's Graphics Shader book
// (when you see "half-vector", that's eye space)
// Technically, a "per-pixel" or a form of Phong shading
// Calcualte the contribution of a light at a vertex
vec3 calcLightColour(in vec3 vecNormal,
	in vec3 vecWorldPosition,
	in int lightID,
	in vec3 matDiffuse, 	// ADDED
	in vec4 matSpecular)	// ADDED
{
vec3 colour = vec3( 0.0f, 0.0f, 0.0f );
	
	vec3 outDiffuse = vec3( 0.0f, 0.0f, 0.0f );
	vec3 outSpecular = vec3( 0.0f, 0.0f, 0.0f );
	

	// Distance between light and vertex (in world)
	vec3 vecToLight = myLight[lightID].position.xyz - vecWorldPosition;
	// How long is this vector?
	float lightDistance = length(vecToLight);
	// Short circuit distance
	if ( myLight[lightID].typeParams.y <  lightDistance )
	{
		return colour;
	}
	
	// The "normalized" vector to the light (don't care about distance, just direction)
	vec3 lightVector = normalize( vecToLight );
	
	// The normalized version of the vertex normal
	vecNormal = normalize(vecNormal);
	
	// How much diffuse light is being "reflected" off the surface 
	float diffFactor = max(0.0f, dot( lightVector, vecNormal ));
	
	
	outDiffuse.rgb = myLight[lightID].diffuse.rgb 		// Light contribution
	                 * matDiffuse.rgb				// Material contribution
					 * diffFactor;						// Factor based on direction

// Simple linear attenuation
//	float attenuation = 1.0f / lightDistance; 
	
	// From the "Shader" book article
	// 1.0 dividedBy ( constant 
	//                + linear * distance 
	//                + quadratic * distance * distance)
	// Our light description is: 
	// 	vec4 attenuation;	// x = constant, 
	//                         y = linear, 
	//                         z = quadratic
	float attenuation = 1.0f / 
	   ( myLight[lightID].attenuation.x // 0  
	   + myLight[lightID].attenuation.y * lightDistance			// Linear
	   + myLight[lightID].attenuation.z * lightDistance * lightDistance );	// Quad
	outDiffuse.rgb *= attenuation;

	
	// Vector from vertex to eye 
	// i.e. this makes the vector base effectively at zero.
	vec3 viewVector = normalize( eyePosition - vecWorldPosition );
	vec3 vecLightReflection = reflect( normalize(lightVector), vecNormal );
	
	float specularShininess = matSpecular.w;	// 64.0f
	vec3 specMatColour = matSpecular.rgb;		// vec3(1.0f, 1.0f, 1.0f);
	
	outSpecular.rgb = pow( max(0.0f, dot(viewVector,vecLightReflection)), 
	                  specularShininess)
			          * specMatColour
				      * myLight[lightID].specular.rgb;// // myLightSpecular;
				   
	outSpecular *= attenuation;
	
	
// For now, to simplify, eliminate the specular
//	colour = outDiffuse + outSpecular;
	colour = outDiffuse;
	
	// Now we have the colour if this was a point light 
	// See if it's a spot light...
	if ( myLight[lightID].typeParams.x == 2.0f ) 			// x = type
	{	// Then it's a spot light! 
		// 		0 = point
		// 		1 = directional
		// 		2 = spot
	    // z angle1, w = angle2		- only for spot
		
		// Vector from the vertex to the light... 
		vec3 vecVertexToLight = vecWorldPosition - myLight[lightID].position.xyz;
		// Normalize to unit length vector
		vec3 vecVtoLDirection = normalize(vecVertexToLight);
		
		float vertSpotAngle = max(0.0f, dot( vecVtoLDirection, myLight[lightID].direction.xyz ));
		// Is this withing the angle1?
		
//		float angleInsideCutCos = cos(myLight[lightID].typeParams.z);		// z angle1
//		float angleOutsideCutCos = cos(myLight[lightID].typeParams.w);		// z angle2
		float angleInsideCutCos = cos(myLight[lightID].typeParams.z);		// z angle1
		float angleOutsideCutCos = cos(myLight[lightID].typeParams.w);		// z angle2
		
		if ( vertSpotAngle <= angleOutsideCutCos )
		{	// NO, it's outside this angle1
			colour = vec3(0.0f, 0.0f, 0.0f );
		}
		else if ( (vertSpotAngle > angleOutsideCutCos ) && 
		          (vertSpotAngle <= angleInsideCutCos) )
		{	// inside the 'penumbra' region
			// Use smoothstep to get the gradual drop off in brightness
			float penRatio = smoothstep(angleOutsideCutCos, 
			                            angleInsideCutCos, 
										vertSpotAngle );          
			
			colour *= penRatio;
		}
//		else if ( vertSpotAngle <= angleInsideCutCos )
//		{
//			// Full brightness .... do nothing
//		}

//		colour.rgb*= 0.0001f;
//		colour.r = 1.0f;

	}//if ( typeParams.x
	
	
	return colour;
}// vec3 calcLightColour(...) 