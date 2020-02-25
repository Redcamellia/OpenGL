#version 430 core

out vec4 color ;
in vec2 texCoord ;

uniform sampler2D ourTexture ;
//uniform vec4 ourColor ;
void main(void)
{
    color = texture(ourTexture ,texCoord) ;
}
