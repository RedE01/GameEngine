#section vertex
#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTextureCoords;

out vec2 textureCoords;

void main() {
    textureCoords = aTextureCoords;

    gl_Position = vec4(aPos, 0.0, 1.0);
}

#section fragment
#version 430 core

out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D u_gPosition;
uniform sampler2D u_gNormal;
uniform sampler2D u_gAlbedo;

void main() {
	vec3 fragPos = texture(u_gPosition, textureCoords).xyz;
	vec3 normal = texture(u_gNormal, textureCoords).xyz;
	vec3 albedo = texture(u_gAlbedo, textureCoords).rgb;

    FragColor = vec4(albedo, 1.0);
}
