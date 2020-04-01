#version 430 core 

in vec3 TexCoord ;

uniform samplerCube skybox ;

out vec4 FragColor ;
void main()
{
    FragColor = texture(skybox , TexCoord);
}