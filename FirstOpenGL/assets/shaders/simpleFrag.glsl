// Fragment shader
#version 400

in vec4 color;					// was vec3
in vec3 vertNormal;
in vec3 vecWorldPosition;
in vec4 uvX2out;			// Added: UV 1 and 2 to fragment
in vec4 FragPosLightSpace;
in mat4 lightSpaceMatrixO;
in float visibility;
//TODO: use this instead of the above 
struct sFBOout
{
	vec4 colour;			// GL_COLOR_ATTACHMENT0
	vec4 normal;			// GL_COLOR_ATTACHMENT1
	vec4 vertexWorldPos;	// GL_COLOR_ATTACHMENT2
};
out sFBOout fragOut;

out vec4 BrightColor;

// The values our OBJECT material
uniform vec4 materialDiffuse;
//uniform vec4 materialAmbient;   		// 0.2
uniform float ambientToDiffuseRatio; 	// Maybe	// 0.2 or 0.3
uniform vec4 materialSpecular;			// rgb = colour of HIGHLIGHT only
										// w = shininess of the 
uniform vec3 eyePosition;				// Camera position

uniform bool bIsDebugWireFrameObject;
uniform bool bIsSecondPass;			// True if the render is doing 2nd pass  
uniform bool getBrightness;
uniform bool isBlur;
uniform bool isBloom;
uniform bool isGreyScale;
uniform bool useFog;
uniform float exposure;
uniform vec3 skycolor;

// Note: this CAN'T be an array (sorry). See 3D texture array
uniform sampler2D texSamp2D00;		// Represents a 2D image
uniform sampler2D texSamp2D01;		// Represents a 2D image
uniform sampler2D texSamp2D02;
uniform float texBlend00;
uniform float texBlend01;
uniform float texBlend02;

uniform bool isASkyBox;				// True samples the skybox texture
uniform samplerCube texSampCube00;
uniform float texCubeBlend00;


// For env. mapping (reflection and refraction)
uniform bool isReflectRefract;
uniform float reflectBlendRatio;		// How much reflection (0-1)
uniform float refractBlendRatio;		// How much refraction (0-1)
uniform float coefficientRefract; 		// coefficient of refraction 

uniform sampler2D texFBOColour2D;
uniform sampler2D texFBONormal2D;
uniform sampler2D texFBOVertexWorldPos2D;
uniform sampler2D textFBODepth2D;

uniform bool isHeightMapped;
uniform bool toonShade;
uniform bool IsDiffuse;
uniform float screenWidth;
uniform float screenHeight;

const float cellLevels = 6.0f;
const float DONT_CALCULATE_LIGHTING = 0.25f;
/*****************************************************/
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

/*******************************************************/
//					SHADOW STUFF					  //
/*****************************************************/

uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform bool useShadows;
uniform bool useShadowMapAsTexture;

float ShadowCalculation(vec4 fragPosLightSpace)
{

	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(textFBODepth2D, projCoords.xy).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// calculate bias (based on depth map resolution and slope)
	vec3 normal = normalize(vertNormal);
	vec3 lightDir = normalize(lightPos - vecWorldPosition);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	// check whether current frag pos is in shadow
	// float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	// PCF
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(textFBODepth2D, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(textFBODepth2D, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if (projCoords.z > 1.0)
		shadow = 0.0;

	return shadow;
}
/*****************************************************/

uniform float weight[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
	if (useShadows) {
		//fragOut.colour = color;
		fragOut.vertexWorldPos = color;
		return;
	}

	//if (isHeightMapped) {
	//	fragOut.colour = color;
	//	return;
	//}


	//fragColourOut = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	fragOut.colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	fragOut.normal = vec4(0.0f, 0.0f, 0.0f, 0.25f);
	fragOut.vertexWorldPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 matSpecular = vec4(1.0f, 1.0f, 1.0f, 34.0f);
	vec3 matDiffuse = vec3(0.0f, 0.0f, 0.0f);


	if (bIsSecondPass)
	{
		vec2 textCoords = vec2(gl_FragCoord.x / screenWidth, gl_FragCoord.y / screenHeight);
		vec3 theColourAtThisPixel = texture(texFBOColour2D, textCoords).rgb;
		vec4 theNormalAtThisPixel = texture(texFBONormal2D, textCoords).rgba;
		vec3 theVertLocWorldAtThisPixel = texture(texFBOVertexWorldPos2D, textCoords).rgb;

		if (isGreyScale) {
			float Y = (0.2126 * theColourAtThisPixel.r) +
				(0.7152 * theColourAtThisPixel.g) +
				(0.0722 * theColourAtThisPixel.b);
			theColourAtThisPixel = vec3(Y, Y, Y);
		}

		if (getBrightness) {

			float filterLimit = 0.1f;
			float filterStep = 0.001f;
			int count = 0;
			vec3 outColour = vec3(0.0f, 0.0f, 0.0f);

			float xMin = -(filterLimit * filterStep);
			float xMax = (filterLimit + 1) * filterStep;
			float yMin = -(filterLimit * filterStep);
			float yMax = (filterLimit + 1) * filterStep;


			//check whether result is higher than some threshold, if so, output as bloom threshold color
			//get the brightness and blur it then add it to the original color
			float brightness = dot(theColourAtThisPixel, vec3(0.2126, 0.7152, 0.0722));
			if (brightness > 0.65) {

				for (float xOff = xMin; xOff < xMax; xOff += filterStep)
				{
					for (float yOff = yMin; yOff < yMax; yOff += filterStep)
					{
						vec2 sampOffset = vec2(xOff, yOff);
						outColour += texture(texFBOColour2D, textCoords + sampOffset).rgb;
						count++;
					}//for ( float yOff
				}//for ( float xOff

				fragOut.colour.rgb = theColourAtThisPixel + outColour;// +outColour;
			}
			else {
				//fragOut.colour = vec4(0.0, 0.0, 0.0, 1.0);
				fragOut.colour.rgb = theColourAtThisPixel;
			}
		}
		else {
			fragOut.colour.rgb = theColourAtThisPixel;
		}


		if (isBlur) {
			float filterLimit = 0.05f;
			float filterStep = 0.001f;
			int count = 0;
			vec3 outColour = vec3(0.0f, 0.0f, 0.0f);

			float xMin = -(filterLimit * filterStep);
			float xMax = (filterLimit + 1) * filterStep;
			float yMin = -(filterLimit * filterStep);
			float yMax = (filterLimit + 1) * filterStep;

			//   *  *    *    *   *
			//
			//   *  *    *    *   *
			//
			//   *  *    X    *   *
			// 
			//   *  *    *    *   *
			// 
			//   *  *    *    *   *

			for (float xOff = xMin; xOff < xMax; xOff += filterStep)
			{
				for (float yOff = yMin; yOff < yMax; yOff += filterStep)
				{

					vec2 sampOffset = vec2(xOff, yOff);

					outColour += texture(texFBOColour2D, textCoords + sampOffset).rgb;

					count++;
				}//for ( float yOff
			}//for ( float xOff

			fragOut.colour.rgb *= 0.0001f;	// make "zero"
			fragOut.colour.rgb += (outColour / float(count));
		}

		fragOut.colour.rgb *= 1.5f;		// dim projector
		fragOut.colour.a = 1.0f;
		return;
	}

	if (isASkyBox)
	{	// Sample from skybox texture and exit
		// I pass texture coords to 'sample' 
		//	returning a colour (at that point in the texture)
		// Note we are using the normals of our skybox object
		//	to determine the point on the inside of the box
		vec4 skyRGBA = texture(texSampCube00, vertNormal.xyz);

		//		fragColourOut.rgb += vec3(0.0f, 1.0f, 0.0f);
		//		fragColourOut[0] = vec4(skyRGBA.rgb, 1.0f);		//gl_FragColor = skyRGBA;
		fragOut.colour = vec4(skyRGBA.rgb, 1.0f);

		fragOut.vertexWorldPos.xyz = vecWorldPosition.xyz;
		fragOut.normal.a = 0.25f;

		return;
	}

	if (isReflectRefract)
	{
		// Have "eyePosition" (camera eye) in WORLD space

		// reFLECTion value 
		vec3 vecReflectEyeToVertex = vecWorldPosition - eyePosition;
		vecReflectEyeToVertex = normalize(vecReflectEyeToVertex);
		vec3 vecReflect = reflect(vecReflectEyeToVertex, vertNormal.xyz);
		// Look up colour for reflection
		//		vec4 rgbReflection = texture( texSampCube00, vecReflect );
		vec4 rgbReflection = texture(texSampCube00, vertNormal.xyz);



		vec3 vecReFRACT_EyeToVertex = eyePosition - vecWorldPosition;
		vecReFRACT_EyeToVertex = normalize(vecReFRACT_EyeToVertex);
		vec3 vecRefract = refract(vecReFRACT_EyeToVertex, vertNormal.xyz,
			coefficientRefract);
		// Look up colour for reflection
		vec4 rgbRefraction = texture(texSampCube00, vecRefract);


		// Mix the two, based on how reflective the surface is
		//	fragColourOut.r = 1.0f;
		//	fragColourOut[0] = (rgbReflection * reflectBlendRatio) + 
		fragOut.colour = (rgbReflection * reflectBlendRatio) +
			(rgbRefraction * refractBlendRatio);

		fragOut.vertexWorldPos.xyz = vecWorldPosition.xyz;
		fragOut.normal.a = 0.25f;
		return;
	}

	// Is this a 'debug' wireframe object, i.e. no lighting, just use diffuse
	if (bIsDebugWireFrameObject)
	{
		fragOut.colour.rgb = materialDiffuse.rgb;			//gl_FragColor.rgb
		fragOut.colour.a = materialDiffuse.a;				//gl_FragColor.a = 1.0f	
		//fragOut.colour.rgb += vec3(0.0f, 0.0f, 1.0f);

		if (useFog) {
			fragOut.colour = mix(vec4(skycolor, 1.0f), fragOut.colour, visibility);
		}

		fragOut.colour * 1.5f;	// Room too bright
		return;		// Immediate return
	}

	// ****************************************************************/
	//uniform sampler2D myAmazingTexture00;
	vec2 theUVCoords = uvX2out.xy;		// use UV #1 of vertex

	vec4 texCol00 = texture(texSamp2D00, theUVCoords.xy);
	vec4 texCol01 = texture(texSamp2D01, theUVCoords.xy);

	//... and so on (to how many textures you are using)
	//	
	// use the blend value to combine textures
	//	vec3 matDiffuse = vec3(0.0f,1.0f,1.0f);
	//	vec3 matDiffuse = vec3(theUVCoords.xy, 0.0f);
	//	matDiffuse += texCol00.rgb;
	if (IsDiffuse) {
		matDiffuse.rgb = materialDiffuse.rgb;
	}
	else {
		matDiffuse.rgb +=
			(texCol00.rgb * texBlend00) +
			(texCol01.rgb * texBlend01);;
	}




	// We will look at specular or gloss maps later, 
	// 	but making the specular white is fine
	float shadow = ShadowCalculation(FragPosLightSpace);
	if (shadow == 1.0f)
	{
		//vec4 depth = texture(textFBODepth2D, theUVCoords);
		fragOut.colour.rgb = matDiffuse.rgb - vec3(0.01);
		//return;
	}
	else {
		////// ****************************************************************/	
		for (int index = 0; index < NUMBEROFLIGHTS; index++)
		{
			 //Old version, which used 'global' diffuse and specular
			//gl_FragColor.rgb += calcLightColour( vertNormal, vecWorldPosition, index );
			fragOut.colour.rgb += calcLightColour(vertNormal,
				vecWorldPosition,
				index,
				matDiffuse,
				matSpecular);
		}
	}



	vec3 ambientContribution = matDiffuse.rgb * ambientToDiffuseRatio;
	fragOut.colour.rgb += ambientContribution.rgb;
	// Add the ambient here (AFTER the lighting)
	// We have materialAmbient, but ambient is often 
	//	just a percentage ratio of the diffuse



	// Transparency value (for alpha blending)
	fragOut.colour.a = materialDiffuse.a;

	//mix the normal color with the skycolor according to the pixel visibility
	if (useFog) {
		fragOut.colour = mix(vec4(skycolor, 1.0f), fragOut.colour, visibility);
	}

	fragOut.normal.rgb = vertNormal.xyz;

	fragOut.vertexWorldPos.xyz = vecWorldPosition.xyz;



	if (useShadowMapAsTexture) {
		//float depthValue = 
		//fragOut.colour = vec4(vec3(depthValue), 1.0);

		fragOut.colour = texture(texFBOVertexWorldPos2D, uvX2out.xy);
		return;
	}

	return;
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
//	

	if (toonShade) {
		float nDotL = dot(vecNormal, lightVector);
		float brightness = max(nDotL, 0.0);
		float level = floor(brightness * cellLevels);
		brightness = level / cellLevels;
		outDiffuse = (brightness * outDiffuse);
		colour = outDiffuse;
	}
	else {
		colour = outDiffuse + outSpecular;
	}

	
	// Now we have the colour if this was a point light 
	// See if it's a spot light...
//	if ( myLight[lightID].typeParams.x == 2.0f ) 			// x = type
//	{	// Then it's a spot light! 
//		// 		0 = point
//		// 		1 = directional
//		// 		2 = spot
//	    // z angle1, w = angle2		- only for spot
//		
//		// Vector from the vertex to the light... 
//		vec3 vecVertexToLight = vecWorldPosition - myLight[lightID].position.xyz;
//		// Normalize to unit length vector
//		vec3 vecVtoLDirection = normalize(vecVertexToLight);
//		
//		float vertSpotAngle = max(0.0f, dot( vecVtoLDirection, myLight[lightID].direction.xyz ));
//		// Is this withing the angle1?
//		
////		float angleInsideCutCos = cos(myLight[lightID].typeParams.z);		// z angle1
////		float angleOutsideCutCos = cos(myLight[lightID].typeParams.w);		// z angle2
//		float angleInsideCutCos = cos(myLight[lightID].typeParams.z);		// z angle1
//		float angleOutsideCutCos = cos(myLight[lightID].typeParams.w);		// z angle2
//		
//		if ( vertSpotAngle <= angleOutsideCutCos )
//		{	// NO, it's outside this angle1
//			colour = vec3(0.0f, 0.0f, 0.0f );
//		}
//		else if ( (vertSpotAngle > angleOutsideCutCos ) && 
//		          (vertSpotAngle <= angleInsideCutCos) )
//		{	// inside the 'penumbra' region
//			// Use smoothstep to get the gradual drop off in brightness
//			float penRatio = smoothstep(angleOutsideCutCos, 
//			                            angleInsideCutCos, 
//										vertSpotAngle );          
//			
//			colour *= penRatio;
//		}
////		else if ( vertSpotAngle <= angleInsideCutCos )
////		{
////			// Full brightness .... do nothing
////		}
//
////		colour.rgb*= 0.0001f;
////		colour.r = 1.0f;
//
//	}//if ( typeParams.x
	

	return colour;
}// vec3 calcLightColour(...) 





