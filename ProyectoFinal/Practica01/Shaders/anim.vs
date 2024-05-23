#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

const float amplitude = 1.0;
const float frequency = 0.01; // Reducir la frecuencia para hacer el movimiento más lento
const float PI = 3.14159;
const float noiseScale = 0.1; // Ajusta este valor según la intensidad deseada del ruido
const float noiseSpeed = 1.0; // Ajusta este valor para controlar la velocidad del ruido

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    float distance = length(aPos);
    float wave = amplitude * sin(-PI * distance * frequency + time);
    float noise = noiseScale * (random(aPos.xy) - 0.5); // Genera un valor de ruido entre -0.5 y 0.5
    float effect = wave + noise;
    gl_Position = projection * view * model * vec4(aPos.x + effect, aPos.y, aPos.z, 1);
    TexCoords = vec2(aTexCoords.x, aTexCoords.y);
}

