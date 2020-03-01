#version 430 core

layout (location = 0 ) in vec3 pos;


uniform mat4 transform ;
uniform mat4 model ;
uniform mat4 view ;
uniform mat4 projection ;
void main(void)
{

        gl_Position = projection  *  view * model *   transform * vec4(pos , 1.0f);

}
