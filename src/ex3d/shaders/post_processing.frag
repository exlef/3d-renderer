#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

const float offset_x = 1.0f / 800.0f;  
const float offset_y = 1.0f / 800.0f;  

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
);

float kernel[9] = float[]
(
    1,  1, 1,
    1, -8, 1,
    1,  1, 1
);

void main()
{
    // vec3 color = vec3(0.0f);
    // for(int i = 0; i < 9; i++)
        // color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernel[i];

    // color = vec3(texture(screenTexture, texCoords));
    // FragColor = vec4(color, 1.0f);
    //

    // float gamma = 2.2;
    // vec3 diffuseColor = pow(texture(screenTexture, texCoords).rgb, vec3(gamma));
    // FragColor = vec4(diffuseColor, 1.0f);
    
    // FragColor = texture(screenTexture, texCoords);

    // Sample the screen texture
    vec3 color = texture(screenTexture, texCoords).rgb;
    
    // Apply gamma correction
    float gamma = 2.2;
    color = pow(color, vec3(1.0/gamma));
    
    FragColor = vec4(color, 1.0);

    //  debug. delete it later.
    // float depthValue = texture(screenTexture, texCoords).r;
    // FragColor = vec4(vec3(depthValue), 1.0);
}