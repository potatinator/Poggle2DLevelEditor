#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;


vec3 aces_tonemap(vec3 color){	
	mat3 m1 = mat3(
        0.59719, 0.07600, 0.02840,
        0.35458, 0.90834, 0.13383,
        0.04823, 0.01566, 0.83777
	);
	mat3 m2 = mat3(
        1.60475, -0.10208, -0.00327,
        -0.53108,  1.10813, -0.07276,
        -0.07367, -0.00605,  1.07602
	);
	vec3 v = m1 * color;    
	vec3 a = v * (v + 0.0245786) - 0.000090537;
	vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
	return pow(clamp(m2 * (a / b), 0.0, 1.0), vec3(1.0 / 2.2));	
}
void main()
{
    
    float exposure = 1.0;
    float contrast = 1.0;
    float brightness = 0.0;
    float saturation = 1.0;
    float gamma = 1.0/2.2;

    FragColor = texture(screenTexture, TexCoords);
    vec3 color = FragColor.rgb;

    color *= exposure;
    //wb
    color = contrast * (color - vec3(0.5)) + vec3(0.5) + vec3(brightness);
    //filter
    color = mix(vec3(dot(color, vec3(0.2126, 0.7152, 0.0722))), color, saturation);
    color = pow(color, vec3(gamma));

//     color = aces_tonemap(color);

    FragColor = vec4(color, 1.0);
    
    
} 