#include <glm/glm.hpp>

void Deffered(glm::vec2 TexCoords, glm::vec3 FragPos, glm::vec3 Normal){
    glm::vec3 gPosition = FragPos;
    glm::vec3 gNormal = normalize(Normal);
    glm::vec4 gAlbedoSpec;
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}