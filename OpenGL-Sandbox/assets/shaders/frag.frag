#version 430 core
struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shineiness;
};

struct Light {
    vec3 ambient ;
    vec3 diffuse ;
    vec3 specular ;
    vec3 position ;
};
uniform sampler2D texture1 ;

uniform Material material ;
uniform Light light ;
out vec4 color ;
in vec3 normal ;
in vec3 FragPos ;
in vec2 texCoords ;
uniform vec3 camera_pos ;
void main(void)
{
    // ambient
    vec3 ambient =  light.ambient * texture(material.diffuse , texCoords).rgb ;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(FragPos - light.position);
    float diff = max(dot(norm , lightDir),0.0f) ;
    vec3 diffuse  = light.diffuse * diff * texture(material.diffuse , texCoords).rgb;


    // specular
    vec3 viewDir = normalize(camera_pos - FragPos) ;
    vec3 ReflectDir ;
    ReflectDir = reflect( -lightDir,norm );
    float fuck_me = dot(viewDir , ReflectDir) ;
    float spec = pow(max(fuck_me, 0.0f ), material.shineiness);
    vec3 specular = light.specular * (spec * material.specular) ;



    vec3 result = ambient+diffuse+specular ;
    color = vec4(result , 1.0f);

}
