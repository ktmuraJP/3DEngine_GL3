#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float luster;
}; 

struct Light {
    vec3 position;
    vec3 color;
};

in VS_OUT{
    vec2 TexCoords;
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
}fs_in;
  
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform sampler2D shadowMap;

float near = 0.1;
float far = 2000.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) * (far + near - z * (far - near));
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 

    float currentDepth = projCoords.z;

    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}
void main()
{
    vec3 FragPos = fs_in.FragPos;
    vec3 Normal = fs_in.Normal;
    vec4 FragPosLightSpace = fs_in.FragPosLightSpace;
    
    vec3 normal = normalize(Normal);
    // ambient
    vec3 ambient = light.color * material.ambient;

    // diffuse 
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.luster);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = light.color * (spec + material.specular);  
        
    vec3 result = (ambient + specular) +diffuse;//ambient + diffuse + specular;
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    float shadow = ShadowCalculation(FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * light.color;    
 
    FragColor =  vec4(vec3(1 + (1/depth/10)),1.0) * vec4(objectColor,1.0) * 
        vec4(result,1.0);// * vec4(lighting, 1.0); 
}
/*


void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * color;

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    

    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}
*/
