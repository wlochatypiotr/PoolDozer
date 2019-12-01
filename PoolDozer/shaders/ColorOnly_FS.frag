#version 430 core
out vec4 color;

layout (location = 6) uniform vec3 incolor;
void main()
{
    color = vec4(incolor, 1.0f); // Set alle 4 vector values to 1.0f
}