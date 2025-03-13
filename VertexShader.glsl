#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 frag_normal; // Pass normal to the fragment shader
out vec3 frag_position; // Pass position to the fragment shader

void main() {
    frag_normal = mat3(transpose(inverse(Model))) * in_normal; // Transform normal
    frag_position = vec3(Model * vec4(in_position, 1.0)); // Calculate position in world space
    gl_Position = Projection * View * Model * vec4(in_position, 1.0); // Final vertex position
}
