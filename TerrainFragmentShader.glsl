#version 330 core

in vec3 frag_position;

out vec4 frag_color;

void main() {
    // Calculăm înălțimea terenului
    float height = frag_position.y;

    // Definim mai multe culori pentru diferite intervale de înălțime
    vec3 lowColor = vec3(0.1, 0.5, 0.2);   // Verde întunecat
    vec3 midColor = vec3(0.3, 0.7, 0.3);   // Verde deschis
    vec3 highColor = vec3(0.8, 0.8, 0.5);  // Galben-verzui
    vec3 peakColor = vec3(1.0, 1.0, 0.8);  // Bej deschis (vârfuri)

    // Amplificăm variațiile prin definirea tranzițiilor
    vec3 finalColor;
    if (height < 0.5) {
        // Interpolare între lowColor și midColor
        finalColor = mix(lowColor, midColor, smoothstep(0.0, 0.5, height));
    } else if (height < 1.0) {
        // Interpolare între midColor și highColor
        finalColor = mix(midColor, highColor, smoothstep(0.5, 1.0, height));
    } else {
        // Interpolare între highColor și peakColor
        finalColor = mix(highColor, peakColor, smoothstep(1.0, 2.0, height));
    }

    // Setăm culoarea finală
    frag_color = vec4(finalColor, 1.0);
}
