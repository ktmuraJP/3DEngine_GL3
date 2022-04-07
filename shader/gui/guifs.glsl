#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec3 objectColor;
uniform float time;
uniform bool outline;

void main()
{   
    vec3 color = texture(screenTexture, TexCoords).rgb;
    //float size = time;//0.0;
    //vec3 color = texture(screenTexture,TexCoords +
    //0.05* vec2(cos(TexCoords.x*size), sin(TexCoords.y*size))).rgb;
    const float offset = 1.0 / 300.0;  

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    float kernel[9] = float[](
        1, 1, 1,
        1,-9, 1,
        1, 1, 1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    //color *= objectColor;
    float d = (col.r + col.g + col.b) / 3.0;
    /*
    if(outline)        
        FragColor = vec4(vec3(1.0 - d),d);
    else
    */
    FragColor = vec4(color, 1.0);
}
