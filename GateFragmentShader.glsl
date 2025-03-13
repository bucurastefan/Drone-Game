#version 330 core

in vec3 fragNormal;
in vec3 fragPos;

uniform vec3 objectColor;
uniform float alpha;

out vec4 FragColor;

void main()
{
    // Culoare simplă, fără iluminare complexă în acest exemplu
    FragColor = vec4(objectColor, alpha);
}
