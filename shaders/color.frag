#version 330 core

out vec4 FragColor;

uniform vec3 color;
uniform bool debug;

void main()
{
    if(debug){
        FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    } else { 
        FragColor = vec4(color, 1.0);
    }
}  