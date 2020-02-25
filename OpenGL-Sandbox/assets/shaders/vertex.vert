#version 430 core

layout (location = 0 ) in vec3 pos;
layout (location = 1 ) in vec2 tex ;
out vec3 rang ;
out vec2 texCoord ;

uniform mat4 transform ;

void main(void)
{

        gl_Position = transform * vec4(pos , 1.0f);
        texCoord = tex ;

}
