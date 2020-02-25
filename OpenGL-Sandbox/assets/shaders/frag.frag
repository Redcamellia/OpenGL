#version 430 core

out vec4 color ;
in vec2 texCoord ;

uniform sampler2D ourTexture ;
//uniform vec4 ourColor ;
void main(void)
{

    color = vec4(1.0f , 0.0 , 0.0 ,1.0f);
    color = texture(ourTexture ,texCoord) ;
}
