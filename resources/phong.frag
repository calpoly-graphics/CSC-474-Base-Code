#version 330 core
in vec3 fragPos;
in vec3 fragNor;
in vec2 fragTex;
in vec3 lightPos;

out vec4 color;

void main() {
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(-fragPos);
    vec3 normal = normalize(fragNor);
    
    // Ambient calculation
    vec3 ambientColor = vec3(0);
    
    // Diffuse calculation
//    float diffuseFactor = max(dot(lightDir, normal), 0); // Clamp to prevent color from reaching back side
    float diffuseFactor = (dot(lightDir, normal) + 1) / 2; // Normalize so the color bleeds onto the back side
    vec3 diffuseColor = vec3(0.8, 0.2, 0.5) * diffuseFactor;
    
    // Specular calculation
    float specularStrength = 0.5f;
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 3.0f;
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0), shininess);
    vec3 specularColor = vec3(1) * specularFactor * specularStrength;

    color = vec4(ambientColor + diffuseColor + specularColor, 1);
}
