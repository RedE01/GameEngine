#section vertex
#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform vec3 u_lightPos;
uniform float u_attenuationConstant;
uniform float u_attenuationLinear;
uniform float u_attenuationQuadratic;

float calculateCutoffDistance(float constant, float linear, float quadratic, float cutoffVal) {
    return (sqrt(linear * linear - 4.0 * quadratic * (constant - 1.0 / cutoffVal)) - linear) / (2 * quadratic);
}

void main() {
    float cutoffDistance = calculateCutoffDistance(u_attenuationConstant, u_attenuationLinear, u_attenuationQuadratic, 0.02);
    vec3 pos = aPos * cutoffDistance + u_lightPos;
    gl_Position = u_projectionMatrix * u_viewMatrix * vec4(pos, 1.0);
}

#section fragment
#version 430 core

out vec4 FragColor;

uniform sampler2D u_gPosition;
uniform sampler2D u_gNormal;
uniform sampler2D u_gAlbedo;

uniform vec3 u_lightPos;
uniform float u_attenuationConstant;
uniform float u_attenuationLinear;
uniform float u_attenuationQuadratic;

uniform vec3 u_cameraPos;

void main() {
    vec2 textureCoords = gl_FragCoord.xy / vec2(640.0, 480.0);
    vec3 fragPos = texture(u_gPosition, textureCoords).xyz;
    vec3 normal = texture(u_gNormal, textureCoords).xyz;
    vec3 albedo = texture(u_gAlbedo, textureCoords).rgb;

    vec3 deltaPos = fragPos - u_lightPos;
    float distSquare = dot(deltaPos, deltaPos);
    float dist = sqrt(distSquare);
    float attenuation = 1.0 / (u_attenuationConstant + u_attenuationLinear * dist + u_attenuationQuadratic * distSquare);

    float ambient = 0.01;

    float diffuseStrength = 0.5;
    vec3 lightDir = normalize(u_lightPos - fragPos);
    float diffuse = max(0.0, dot(lightDir, normal)) * diffuseStrength;

    float specularStrength = 0.8;
    vec3 viewDir = normalize(u_cameraPos - fragPos);
    vec3 halfDir = (viewDir + lightDir) / 2.0;
    float specular = pow(max(0.0, dot(halfDir, normal)), 16) * specularStrength;

    float brightness = ambient + diffuse + specular;

    FragColor = vec4(albedo * brightness * attenuation, 1.0);
}
