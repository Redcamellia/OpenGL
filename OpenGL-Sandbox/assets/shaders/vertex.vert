#version 430 core

layout (location = 0 ) in vec3 pos;
layout (location = 1 ) in vec3 norm ;

out vec3 normal ;
out vec3 FragPos ;

uniform mat4 model ;
uniform mat4 view ;
uniform mat4 projection ;
void main(void)
{
        FragPos = vec3(model * vec4(pos , 1.0f));
        gl_Position = projection  *  view * vec4(FragPos , 1.0f);
        normal = mat3(transpose(inverse(model))) * norm ;
}
