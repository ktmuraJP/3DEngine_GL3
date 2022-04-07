#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D _texture;
uniform vec3 ParticleColor;

void main()
{
    vec4 tcolor = texture(_texture, TexCoords) * vec4(ParticleColor, 1.0);

    if(tcolor.a == 0.0)
        discard;
    FragColor = tcolor;
} 
