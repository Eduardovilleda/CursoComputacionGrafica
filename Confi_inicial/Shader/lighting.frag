#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;       // 1er cubo de luz
uniform Light secondLight; // 2do cubo de luz

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform sampler2D texture_diffusse;

float distance = length(light.position - FragPos); // Calcula la distancia entre la luz y el fragmento
float attenuation = 1.0f / (distance * distance);

void main()
{
    // Luz ambiental
    vec3 ambient = light.ambient * material.diffuse;
    vec3 secondAmbient = secondLight.ambient * material.diffuse;

    // Luz difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse * attenuation;

    // Luz especular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular) * attenuation;

    // Segunda luz: Luz difusa
    vec3 secondNorm = normalize(Normal);
    vec3 secondLightDir = normalize(secondLight.position - FragPos);
    float secondDiff = max(dot(secondNorm, secondLightDir), 0.0);
    vec3 secondDiffuse = secondLight.diffuse * secondDiff * material.diffuse;

    // Segunda luz: Luz especular
    vec3 secondViewDir = normalize(viewPos - FragPos);
    vec3 secondReflectDir = reflect(-secondLightDir, secondNorm);
    float secondSpec = pow(max(dot(secondViewDir, secondReflectDir), 0.0), material.shininess);
    vec3 secondSpecular = secondLight.specular * (secondSpec * material.specular);

    vec3 result = ambient + diffuse + specular + secondAmbient + secondDiffuse + secondSpecular;
    color = vec4(result, 1.0f) * texture(texture_diffusse, TexCoords);

}