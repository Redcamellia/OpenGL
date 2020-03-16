#version 430 core

out vec4 color ;
uniform sampler2D texture1 ;
in vec2 texCoords ;
void main(void)
{
    color = texture(texture1 , texCoords);
}
