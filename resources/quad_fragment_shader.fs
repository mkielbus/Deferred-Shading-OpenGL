#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

// gBuffer tekstury
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 Position;
    vec3 Color;

    float Linear;
    float Quadratic;
    float SpecExp;
};

uniform Light lights[5];
uniform vec3 viewPos;

void main()
{
    // Pozyskanie danych z G-Buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // Ambient
    vec3 ambient = 0.1 * Albedo;

    for(int iter = 0; iter < 5; iter++)
    {
    // Diffuse
    vec3 lightDir = normalize(lights[iter].Position - FragPos);
    float diff = abs(dot(Normal, lightDir));
    vec3 diffuse = diff * lights[iter].Color * Albedo;
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    // vec3 tmpDir = normalize(lightDir + viewDir);
    float spec = pow(abs(dot(viewDir, reflectDir)), lights[iter].SpecExp);
    vec3 specular = lights[iter].Color * spec * Specular;
    
    // Attenuation
    float distance = length(lights[iter].Position - FragPos);
    float attenuation = 1.0 / (1.0 + lights[iter].Linear * distance + lights[iter].Quadratic * (distance * distance));
    
    diffuse *= attenuation;
    specular *= attenuation;

    ambient += diffuse + specular;

    }

    //vec3 result = ambient + diffuse + specular;
    vec3 result = ambient;
    FragColor = vec4(result, 1.0);
}
