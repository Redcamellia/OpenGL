#version 430 core
struct Material {

    float shineiness;
};

uniform sampler2D m_diffuse ;
uniform sampler2D m_specular ;

struct Light {
    vec3 ambient ;
    vec3 diffuse ;
    vec3 specular ;
    vec3 position ;

    float constant ;
    float linear ;
    float quadratic ;

};
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
    vec3 ambient =  light.ambient * vec3(texture(m_diffuse , texCoords)).rgb ;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position-FragPos );
    float diff = max(dot(norm , lightDir),0.0f) ;
    vec3 diffuse  = diff * texture(m_diffuse , texCoords).rgb * light.diffuse;



    float distance = length(FragPos - light.position);
    float attenuation = 1.0 / (light.constant +
                               light.linear * distance +
                               light.quadratic * (distance * distance));
    // specular
    vec3 viewDir = normalize(camera_pos - FragPos) ;
    vec3 ReflectDir ;
    ReflectDir = reflect( -lightDir,norm );
    float zarb = dot(viewDir , ReflectDir) ;
    float spec = pow(max(zarb, 0.0f ), material.shineiness );
    vec3 specular = spec * light.specular * texture(m_specular , texCoords).rgb;

    ambient *= attenuation ;
    diffuse *= attenuation ;
    specular *= attenuation ;



    vec3 result =ambient+diffuse+specular ;
    color = vec4(result , 1.0f);

}
