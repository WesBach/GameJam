#version 400 
in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoords;

struct sFBOout
{
	vec4 FragPos;
	vec4 Normal;
	vec4 TexCoords;
	vec4 WorldPostition;
};
out sFBOout fragOut;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    fragOut.FragPos.xyz = vec3(model * vec4(aPos, 1.0));   
    fragOut.TexCoords.xy = aTexCoords;
        
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    fragOut.Normal.xyz = normalize(normalMatrix * aNormal);
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}