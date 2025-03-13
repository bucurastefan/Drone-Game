#version 330 core

in vec3 frag_normal;
in vec3 frag_position;

uniform vec3 objectColor; // Color for the object
out vec4 frag_color;

void main() {
    // Simple lighting for demonstration
    vec3 light_dir = normalize(vec3(-1.0, -1.0, -1.0));
    float diff = max(dot(normalize(frag_normal), -light_dir), 0.0);

    // Final color combines object color with diffuse lighting
    vec3 result = objectColor * diff + objectColor * 0.2; // Add some ambient light
    frag_color = vec4(result, 1.0);
}
