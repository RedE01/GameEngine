#pragma once

static const char* defaultShaderVertexSource = R"END(
    #version 430 core

    layout (location = 0) in vec3 aPos;

    uniform mat4 u_modelMatrix;
    uniform mat4 u_viewMatrix;
    uniform mat4 u_projectionMatrix;

    uniform uint u_lightType;
    uniform vec3 u_lightPos;

    uniform float u_pointLightRange;
    out float pointLightLinearConstant;
    out float pointLightQuadraticConstant;

    void main() {
        if(u_lightType == 0) { // Point light
            vec3 pos = aPos * u_pointLightRange + u_lightPos;
            pointLightLinearConstant = 20.0 / u_pointLightRange;
            pointLightQuadraticConstant = 512.0 / (u_pointLightRange * u_pointLightRange);

            gl_Position = u_projectionMatrix * u_viewMatrix * vec4(pos, 1.0);
        }
        else if(u_lightType == 1) { // Direction light
            gl_Position = vec4(aPos, 1.0);
        }
    }
)END";

static const char* defaultShaderFragmentSource = R"END(
#version 430 core

    out vec4 FragColor;

    uniform sampler2D u_gPosition;
    uniform sampler2D u_gNormal;
    uniform sampler2D u_gAlbedo;

    uniform uint u_lightType;

    // Point light uniforms
    uniform vec3 u_lightPos;
    uniform float u_pointLightRange;

    // Directional light uniforms
    uniform vec3 u_lightDir;
    uniform float u_lightIntensity;

    uniform vec3 u_cameraPos;
    uniform vec2 u_viewportSize;

    // Constants
    in float pointLightLinearConstant;
    in float pointLightQuadraticConstant;

    void main() {
        vec2 textureCoords = gl_FragCoord.xy / u_viewportSize;
        vec3 fragPos = texture(u_gPosition, textureCoords).xyz;
        vec3 normal = texture(u_gNormal, textureCoords).xyz;
        vec3 albedo = texture(u_gAlbedo, textureCoords).rgb;

        float attenuation;
        vec3 lightDir;
        if(u_lightType == 0) { // Point light
            lightDir = normalize(u_lightPos - fragPos);

            vec3 deltaLightFragPos = u_lightPos - fragPos;
            float dist = distance(u_lightPos, fragPos);
            float distance2 = dist * dist;
            attenuation = 1.0 / (1.0 + pointLightLinearConstant * dist * pointLightQuadraticConstant * distance2);
        }
        else if(u_lightType == 1) { // Directional light
            lightDir = u_lightDir;
            attenuation = u_lightIntensity;
        }

        float ambient = 0.8;

        float diffuseStrength = 0.8;
        float diffuse = max(0.0, dot(lightDir, normal)) * diffuseStrength;

        float specularStrength = 0.8;
        vec3 viewDir = normalize(u_cameraPos - fragPos);
        vec3 halfDir = (viewDir + lightDir) / 2.0;
        float specular = pow(max(0.0, dot(halfDir, normal)), 16) * specularStrength;

        float brightness = ambient + diffuse + specular;

        FragColor = vec4(albedo * brightness * attenuation, 1.0);
    }
)END";
