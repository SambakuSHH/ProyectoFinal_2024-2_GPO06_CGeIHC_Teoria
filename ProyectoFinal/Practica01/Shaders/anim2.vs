#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

const float amplitude = 0.3; // Amplitud del movimiento
const float frequency = 0.1; // Frecuencia del movimiento
const float offset = 1.0; // Desplazamiento lateral
const float speed = 4.0; // Velocidad del movimiento

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
    float wave = amplitude * sin(frequency * time * speed); // Función sinusoidal modificada
    vec3 newPosition = aPos + vec3(0.0, 0.0, wave + offset); // Aplicar el desplazamiento lateral
    
    gl_Position = projection * view * model * vec4(newPosition, 1.0);
    TexCoords = aTexCoords;
}

