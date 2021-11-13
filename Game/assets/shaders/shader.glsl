#section vertex
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoords;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec3 fragPos;
out vec3 normal;
out vec2 textureCoords;

void main() {
    vec4 fragPosVec4 = u_modelMatrix * vec4(aPos, 1.0);
    fragPos = fragPosVec4.xyz;
    normal = mat3(transpose(inverse(u_modelMatrix))) * aNormal;
    textureCoords = aTextureCoords;

    gl_Position = u_projectionMatrix * u_viewMatrix * fragPosVec4;
}

#section fragment
#version 430 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 textureCoords;

uniform vec3 u_cameraPos;
uniform sampler2D u_texture;

vec3 lightPos = vec3(0.0, 0.0, 10.0);

void main() {
    float ambient = 0.01;

    float diffuseStrength = 0.5;
    vec3 lightDir = normalize(lightPos - fragPos);
    float diffuse = max(0.0, dot(lightDir, normal)) * diffuseStrength;

    float specularStrength = 0.8;
    vec3 viewDir = normalize(u_cameraPos - fragPos);
    vec3 halfDir = (viewDir + lightDir) / 2.0;
    float specular = pow(max(0.0, dot(halfDir, normal)), 16) * specularStrength;

    float brightness = ambient + diffuse + specular;

    vec3 albedo = texture(u_texture, textureCoords).xyz;
    FragColor = vec4(albedo * brightness, 1.0);
}