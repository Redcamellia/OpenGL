#version 410 core

layout (location = 0 ) in vec3 pos;
layout (location = 1 ) in vec3 norm ;
layout (location = 2 ) in vec2 texCoord ;
out vec3 normal ;
out vec3 FragPos ;
out vec2 texCoords ;
uniform mat4 model ;
uniform mat4 view ;
uniform mat4 projection ;
void main(void)
{
        FragPos = vec3(model * vec4(pos , 1.0f));
        gl_Position = projection  *  view *  vec4(FragPos , 1.0f);
        texCoords = vec2(texCoord.x , texCoord.y) ;
        normal = mat3(transpose(inverse(model))) * norm ;
}
