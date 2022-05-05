#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec4 FragPos;

struct Light {
    vec3 lightPos;
    vec3 lightColor;
    mat4 lightSpace;
    sampler2D shadowMap;
    bool enabled;
};

uniform int numLights;
uniform Light lights[2];

float ShadowCalc(vec4 fragPosLightSpace, int i) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(lights[i].shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main() {
    vec3 normal = normalize(Normal);
    vec3 color = vec3(0);
    for (int i = 0; i < numLights; i++) {
        vec3 ambient = 0.15 * lights[i].lightColor;
        vec3 lightDir = normalize(lights[i].lightPos - FragPos.xyz);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * lights[i].lightColor;;
        vec4 fragPosLightSpace = lights[i].lightSpace * FragPos;
        float shadow = ShadowCalc(fragPosLightSpace, i);
        color += (ambient + (1.0 - shadow) * diffuse);
    }
    color /= numLights;
    FragColor = vec4(color, 1);
}
