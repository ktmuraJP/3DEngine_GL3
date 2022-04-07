#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} gs_in[];

out vec2 TexCoords; 
out vec3 FragPos; 
out vec3 Normal;
out vec4 FragPosLightSpace;
uniform float step;

vec4 explode(vec4 position, vec3 normal)
{
    vec3 direction = normal * step; 
    //vec3 direction = vec3(1.0,0.0,1.0); 
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {    

    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].TexCoords;
    FragPos = gs_in[0].FragPos;
    Normal = gs_in[0].Normal;
    FragPosLightSpace = gs_in[0].FragPosLightSpace;
    EmitVertex();
    
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].TexCoords;
    FragPos = gs_in[1].FragPos;
    Normal = gs_in[1].Normal;
    FragPosLightSpace = gs_in[1].FragPosLightSpace;
    EmitVertex();
    
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].TexCoords;
    FragPos = gs_in[2].FragPos;
    Normal = gs_in[2].Normal;
    FragPosLightSpace = gs_in[2].FragPosLightSpace;
    EmitVertex();
    
    EndPrimitive();
}
