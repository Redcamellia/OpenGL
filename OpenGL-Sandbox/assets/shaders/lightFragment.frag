#version 410 core

out vec4 color ;
uniform sampler2D texture1 ;
in vec2 texCoords ;
void main(void)
{
    color = vec4(0.5f , 0.17f , 0.17f , 0.7f);
}
