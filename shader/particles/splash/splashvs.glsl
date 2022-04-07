#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 position;
void main()
{
    TexCoords = aTexCoord;
    vec4 view_pos = view * model * vec4(position,1.0);
    float dist  = 1.0;//-view_pos.z;
    gl_Position = projection * ( view_pos + model * vec4(aPos.xy * dist, 0.0, 0.0));
}
