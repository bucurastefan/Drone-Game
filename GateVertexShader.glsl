#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColor;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 fragNormal;
out vec3 fragPos;

void main()
{
    // Calculăm poziția fragmentului și vectorul normal
    fragPos = vec3(Model * vec4(inPosition, 1.0));
    fragNormal = mat3(transpose(inverse(Model))) * inNormal;

    gl_Position = Projection * View * Model * vec4(inPosition, 1.0);
}
