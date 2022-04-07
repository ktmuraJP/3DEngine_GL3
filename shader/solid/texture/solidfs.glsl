#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform vec3 objectColor;
uniform sampler2D texture1;
void main()
{
    
    FragColor = texture(texture1, TexCoord)* vec4(objectColor,1.0);
}
