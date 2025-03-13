#version 330 core

layout(location = 0) in vec3 in_position;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 frag_position;

// Funcție de noise
float noise(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    vec3 position = in_position;

    // Parametrii pentru noise
    float frequency = 0.02;  // Frecvență mică pentru variații mai lente
    float amplitude = 1.0;   // Amplitudine redusă pentru denivelări mai mici

    // Modificăm înălțimea vertecșilor folosind noise
    position.y = noise(position.xz * frequency) * amplitude;

    frag_position = position;
    gl_Position = Projection * View * Model * vec4(position, 1.0);
}
