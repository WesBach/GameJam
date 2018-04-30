// Vertex shader
#version 420

//uniform mat4 MVP;
uniform mat4 mModel;
//uniform mat4 mModelRotationOnly;
uniform mat4 mView;
uniform mat4 mProjection;
uniform mat4 lightSpaceMatrix;

//height mapping
uniform bool isHeightMapped;
uniform sampler2D textHeightMap;

//noise generation
uniform bool generateNoise;
uniform float noiseOffset;

// "Vertex" attribute
//attribute vec4 vCol;		// was: vec3
//attribute vec3 vPos;		// was: vec2 vPos
//attribute vec3 vNorm;		// Vertex normal
//attribute vec4 uvX2;		// Added: UV 1 and 2

layout(location = 0) in vec4 vCol;		// was: vec3
layout(location = 1) in vec3 vPos;		// was: vec2 vPos
layout(location = 2) in vec3 vNorm;		// Vertex normal
layout(location = 3) in vec4 uvX2;		// Added: UV 1 and 2
layout(location = 7) in vec3 instancePosition;

out vec4 color;				// was: vec4
out vec3 vertNormal;		// Also in "world" (no view or projection)
out vec3 vecWorldPosition;	// 
out vec4 uvX2out;			// Added: UV 1 and 2 to fragment
out vec3 fTangent;		// For bump (or normal) mapping
out vec3 fBitangent;	// For bump (or normal) mapping
out vec4 FragPosLightSpace;
out mat4 lightSpaceMatrixO;
out float visibility;

in vec4 vBoneIDs_x4;		// IS OK. Note these are only used in the vertex shader
in vec4 vBoneWeights_x4;	// IS OK. Note these are only used in the vertex shader  
in vec3 vTangent;		// For bump (or normal) mapping
in vec3 vBitangent;		// For bump (or normal) mapping
 
// For skinned mesh
const int MAXNUMBEROFBONES = 100;
uniform mat4 bones[MAXNUMBEROFBONES];
// Pass the acutal number you are using to control how often this loops
uniform int numBonesUsed;			
uniform bool bIsASkinnedMesh;	// True to do the skinned mesh
uniform bool bIsADepthBuffer;
uniform bool bCalculateShadows;
uniform bool isNoiseGenerating;
uniform bool isInstanced;
uniform float deltaTime;
uniform sampler2D texSamp2D01;

float rand(float n){return fract(sin(n) * 43758.5453123);}


const float density = 0.003;
const float gradient = 2.5;



vec2 hash( vec2 p )
{
	p = vec2( dot(p,vec2(127.1,311.7)),
			 dot(p,vec2(269.5,183.3)) );
	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float noise( in vec2 p )
{
	const float K1 = 0.366025404; // (sqrt(3)-1)/2;
	const float K2 = 0.211324865; // (3-sqrt(3))/6;
	
	vec2 i = floor( p + (p.x+p.y)*K1 );
	
	vec2 a = p - i + (i.x+i.y)*K2;
	vec2 o = (a.x>a.y) ? vec2(1.0,0.0) : vec2(0.0,1.0);
	vec2 b = a - o + K2;
	vec2 c = a - 1.0 + 2.0*K2;
	
	vec3 h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );
	
	vec3 n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));
	
	return dot( n, vec3(70.0) );
}

float fbm(vec2 uv)
{
	float f;
	mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
	//f  = 0.5000*noise( uv ); uv = m*uv;
	f += 0.2500*noise( uv ); uv = m*uv;
	f += 0.1250*noise( uv ); uv = m*uv;
	f += 0.0625*noise( uv ); uv = m*uv;
	f = 0.5 + 0.5*f;
	return f;
}


void main()
{
	vec4 position;

	if (isInstanced) {
		position = vec4(vPos + instancePosition, 1.0);
	}
	else {
		position = vec4(vPos, 1.0);
	}

	//vec4 vertPosition = vec4(vPos, 1.0f);
	mat4 matModel = mModel;
	
	if(bIsADepthBuffer){

		if (isHeightMapped) {
			//change the height of the object 
			vec4 colorAtHeightMapPixel = texture(texSamp2D01, uvX2.xy);
			position.y -= colorAtHeightMapPixel.r * 2000.0f;
		}

		gl_Position = lightSpaceMatrix * mModel * position;
		//using this for moving the position to be accessed by texture
		color = lightSpaceMatrix * mModel * position;
		return;
	}
	
	if (isHeightMapped) {
	//change the height of the object 
		vec4 colorAtHeightMapPixel = texture(texSamp2D01, uvX2.xy);
		position.y -= colorAtHeightMapPixel.r * 2000.0f;
		//color = colorAtHeightMapPixel;
		//mat4 MVP = mProjection * mView * mModel;
		//gl_Position = MVP * position;
		//return;
	}
	
	if(isNoiseGenerating){
		vec2 uvCoords = uvX2.xy;
		uvCoords.x += noiseOffset;
		uvCoords.y += noiseOffset;
		//calculate random indexes to get the data from 
		 vec4 colorAtHeightMapPixel = texture(texSamp2D01, uvCoords);
		
		position.y += colorAtHeightMapPixel.r * 40.0f;
	}
	
	if ( ! bIsASkinnedMesh )
	{
		// Calculate the model view projection matrix here
		mat4 MVP = mProjection * mView * matModel;
		//gl_Position = MVP * vec4(vertPosition, 1.0f);
		gl_Position = MVP * position;
	
		// Calculate vertex and normal based on ONLY world 
		//fVecWorldPosition = vec3( matModel * vec4(vertPosition, 1.0f) ).xyz;
		vecWorldPosition = vec3( matModel * position ).xyz;
	
		// Inv Tran - strips translation and scale from model transform
		// Alternative is you pass a "rotation only" model mat4
		mat4 mWorldInvTranspose = inverse(transpose(matModel));		/*Now passed in*/
	
		// Was: MVP * vNorm;
		// This normal is in "world space" but only has rotation
		vertNormal = vec3( mWorldInvTranspose * vec4(vNorm, 1.0f) ).xyz;	

		//	mat4 mWorldInverseTranspose = inverse(transpose(matModel));
		//	vertNormal = vec3( mWorldInverseTranspose * vec4(vNorm, 1.0f) ).xyz;	
			
		// Pass the tangent and bi-tangent out to the fragment shader
		fTangent = vTangent;
		fBitangent = vBitangent;

	}//if ( ! bIsASkinnedMesh )
	
	if ( bIsASkinnedMesh )
	{
		// If a single bone... 
//		mat4 BoneTransform = mat4(1.0f);
//		BoneTransform = bones[ int(vBoneIDs_x4[0]) ] * vBoneWeights_x4[0];		
//		BoneTransform += bones[ int(vBoneIDs_x4[1]) ] * vBoneWeights_x4[1];
//		BoneTransform += bones[ int(vBoneIDs_x4[2]) ] * vBoneWeights_x4[2];
//		BoneTransform += bones[ int(vBoneIDs_x4[3]) ] * vBoneWeights_x4[3];

		mat4 BoneTransform = bones[ int(vBoneIDs_x4[0]) ] * vBoneWeights_x4[0];
		BoneTransform += bones[ int(vBoneIDs_x4[1]) ] * vBoneWeights_x4[1];
		BoneTransform += bones[ int(vBoneIDs_x4[2]) ] * vBoneWeights_x4[2];
		BoneTransform += bones[ int(vBoneIDs_x4[3]) ] * vBoneWeights_x4[3];
				
	//	matrixWorld = BoneTransform * matrixWorld;
	
		position = BoneTransform * position;
		
		mat4 matMVP = mProjection * mView * mModel;		// m = p * v * m;
		
		// Final screen space position	
		gl_Position = matMVP * position;	
		
//		gl_Position = mProjection * mView * mModel * BoneTransform * vertPosition;	
		
		// Additional transformations so lighthing (normal) will work
		//mat4 matMV = matrixView * matrixWorld;	// model-view matrix
		//fVecWorldView = vec3(matMV * vertPosition).xyz;	
		
		// Inverse transform to keep ONLY rotation...
		mat4 matNormal = inverse( transpose(BoneTransform * mModel) );
		//
		vertNormal = mat3(matNormal) * normalize(vNorm.xyz);
		fTangent = 	mat3(matNormal) * normalize(vTangent.xyz);
		fBitangent = 	mat3(matNormal) * normalize(vBitangent.xyz);
		
		vecWorldPosition = (mModel * position).xyz;

	}//if ( bIsASkinnedMesh )
	
	// Calculate the model view projection matrix here
	mat4 MVP = mProjection * mView * mModel;



	gl_Position = MVP * vec4(position.xyz, 1.0f);
	
	// Calculate vertex and normal based on ONLY world 
	vecWorldPosition = vec3( mModel * vec4(position.xyz, 1.0f) ).xyz;
	
	// Inv Tran - strips translation and scale from model transform
	// Alternative is you pass a "rotation only" model mat4
	mat4 mWorldInTranspose = inverse(transpose(mModel));
	
	// Was: MVP * vNorm;
	// This normal is in "world space" but only has rotation
	vertNormal = vec3( mWorldInTranspose * vec4(vNorm, 1.0f) ).xyz;	
	
	vec3 FragPos = vec3(mModel * vec4(vPos, 1.0));	
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

	lightSpaceMatrixO = lightSpaceMatrix;

	//get the position relative to the camera
	vec4 positionRelativeToCamera = mView * vec4(vecWorldPosition,1.0f);
	float distance = length(positionRelativeToCamera.xyz);
	visibility = exp(-pow((distance*density), gradient));
	distance = clamp(visibility, 0, 1);

	color = vCol;	
	uvX2out = uvX2;			// Sent to fragment shader

}

