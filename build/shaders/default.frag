#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform float time;

void main() {
    // A simple color based on position and time
    vec3 color = 0.5 + 0.5 * cos(time + FragPos.xyx + vec3(0,2,4));
    
    // Basic lighting
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * color;
    vec3 ambient = 0.1 * color;
    
    FragColor = vec4(ambient + diffuse, 1.0);
}
