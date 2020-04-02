#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position ;

uniform samplerCube skybox;
uniform vec3 cameraPos ;

void main()
{   float ratio = 1.0f / 1.52f ;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I , normalize(Normal) , ratio);
    FragColor = vec4(texture(skybox , R).rgb , 1.0f);
}