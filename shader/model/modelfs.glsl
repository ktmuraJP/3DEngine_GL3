#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;


uniform sampler2D shadowMap;
//uniform sampler2D texture_diffuse1;
uniform sampler2D textureData[10];

uniform vec3 objectColor;
uniform float luster;
uniform float alpha;


uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;
uniform float near_plane;
uniform int use_shadow;
uniform int texture_index;


float near = 0.1;
float far = 2000.0;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
    vec4 seed4 = vec4(seed,i);
    float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) * (far + near - z * (far - near));
}

float ShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowmap)
{   
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowmap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec4 FragPosLightSpace = fs_in.FragPosLightSpace;
    
    float bias =  max(0.006 * (1.0 - dot(normal, lightDir)), 0.0006);
    float shadow = 1.0; 
 
    vec2 texelSize = 0.5 / textureSize(shadowmap, 0);

    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            float pcfDepth = texture(shadowmap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 0.75 : 0.0;  
        }
    }
    shadow /= 10.0;
    return shadow;
}

void main()
{           
    
    vec3 color;
    if(texture_index < 0){
        color = objectColor;
        //color = texture(mytexture,fs_in.TexCoords).rgb;
    }else{
        //color = objectColor;
        //color = texture(texture_diffuse1,fs_in.TexCoords).rgb;
        /*
            テクスチャはID振り分けで設定（textureData[x]) x --> 使うID
            おそらくそんなにテクスチャを使わないのでID固定、画像変更するときは
            書き換えでいいと思う。
            現在
               ０番 ---> uvmap.png
               １番 ---> undef
               ２番 ---> undefined
               … 
        */
        if(texture_index == 0)
            color = texture(textureData[0],fs_in.TexCoords).rgb;
        if(texture_index == 1)
            color = texture(textureData[1],fs_in.TexCoords).rgb;
    }
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);
    vec3 lpos = vec3(lightPos.x, lightPos.y + 1000.0f, lightPos.z);

    // ambient
    vec3 ambient = 0.3 * color;

    // diffuse
    vec3 lightDir = normalize(lpos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), luster);
    vec3 specular = spec * lightColor;    

    // calculate shadow
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    vec3 depthshade = vec3(1.0);// + (1.0 / depth / 10.0));
    
    
    float shadow = depth > 0.25 ? ShadowCalculation(fs_in.FragPosLightSpace,shadowMap) : 0.1; 
    vec3 lighting = use_shadow == 0 ? 
        (ambient + (1.0 - shadow) * (diffuse + specular)) * color : 
        (ambient + diffuse + specular) * color ;    

    vec4 final = vec4(lighting, alpha);    
    FragColor = final;
}
