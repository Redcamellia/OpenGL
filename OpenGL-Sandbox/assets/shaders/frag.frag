#version 430 core

out vec4 color ;
in vec3 normal ;
in vec3 FragPos ;
uniform vec3 lightColor ;
uniform vec3 ourColor ;
uniform vec3 lightPos ;
uniform vec3 camera_pos ;
void main(void)
{
    // ambient
    float ambientSTR = 0.1f ;
    vec3 ambient = ambientSTR * lightColor ;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(FragPos - lightPos);
    float diff = max(dot(norm , lightDir),0.0f) ;
    vec3 diffuse  =  diff * lightColor ;


    // specular

    float specularStrength = 0.5f ;
    vec3 viewDir = normalize(camera_pos - FragPos) ;

    vec3 ReflectDir ;
//    if(dot(lightDir , norm) < 0.0){
    ReflectDir = reflect( -lightDir,norm );
//}
//    else{
//    ReflectDir = vec3(0.0 , 0.0 , 0.0);
//    }

    float fuck_me = dot(viewDir , ReflectDir) ;
    float spec = pow(max(fuck_me, 0.0f ), 128);
    vec3 specular = specularStrength * spec * lightColor ;



    vec3 result =  (/*ambient+diffuse+*/specular) * ourColor;
    color = vec4(result , 1.0f);

}
