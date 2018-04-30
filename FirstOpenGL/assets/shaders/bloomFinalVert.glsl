#version 400
in vec3 aPos;
in vec2 aTexCoords;

out vec2 TexCoords;
out vec4 Position;

void main()
{
    TexCoords = aTexCoords;
    Position = vec4(aPos, 1.0);
}