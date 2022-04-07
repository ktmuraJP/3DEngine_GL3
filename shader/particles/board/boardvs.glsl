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
    vec4 view_pos = view * model *  vec4(1.0);
    float dist  = -view_pos.z;
    //vec4 const_billboard = projection * ( view_pos + model * vec4(aPos.xy * dist, 0.0, 0.0));
    vec4 const_billboard = projection * ( view_pos + model * vec4(aPos.xy * dist, 0.0, 0.0));
    gl_Position = const_billboard;
}
