
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

// mat4 instanceMatrix ;
void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0f); 
}