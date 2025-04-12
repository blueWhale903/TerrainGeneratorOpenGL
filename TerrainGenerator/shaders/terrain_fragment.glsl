// shaders/terrain_fragment.glsl
#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Terrain coloring based on height
    float height = FragPos.y;
    vec3 terrainColor;

    if (height < 10.0) {
        terrainColor = mix(vec3(0.0, 0.3, 0.7), vec3(0.9, 0.8, 0.5), height / 10.0); // Water to sand
    }
    else if (height < 20.0) {
        terrainColor = mix(vec3(0.2, 0.6, 0.2), vec3(0.5, 0.5, 0.2), (height - 10.0) / 10.0); // Grass to dirt
    }
    else {
        terrainColor = mix(vec3(0.5, 0.5, 0.2), vec3(0.8, 0.8, 0.8), (height - 20.0) / 30.0); // Dirt to snow
    }

    vec3 result = (ambient + diffuse + specular) * terrainColor;
    FragColor = vec4(result, 1.0);
}