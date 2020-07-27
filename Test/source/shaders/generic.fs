#version 330 core
in VS_OUT{
	vec3 fragment_position;
    vec3 fragment_normal;
	vec3 fragment_color;
    vec2 fragment_texCoords;
}fs_in;


struct Material {
	//vec3 ambient;
	vec3 specular;
	sampler2D diffuse;

	float shininess;
};

struct Light {
	vec3 position;
	//vec3 direction;
	//float cutOff;
	//float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

uniform vec3 view_position;
uniform Light light;
uniform Material material;
uniform int byColor = 0;
uniform samplerCube shadowMap;
uniform float far_plane;


out vec4 color;
float ShadowCalculation(vec3 fragPos, Light light, samplerCube shadowMap, float far_plane);
vec3 pointLightRender(Light light, Material material, vec3 view_position, int byColor, vec3 fragment_position, vec3 fragment_normal, vec3 fragment_color, vec2 fragment_texCoords, samplerCube shadowMap);


void main()
{
	vec3 result = pointLightRender(light, material, view_position, byColor, fs_in.fragment_position, fs_in.fragment_normal, fs_in.fragment_color, fs_in.fragment_texCoords, shadowMap);
	color = vec4(result, 1.0);
}

vec3 pointLightRender(Light light, Material material, vec3 view_position, int byColor, vec3 fragment_position, vec3 fragment_normal, vec3 fragment_color, vec2 fragment_texCoords, samplerCube shadowMap){
	vec3 result;
    // ambient
    vec3 ambient;
	if(byColor==0)
		ambient = light.ambient *  vec3(texture(material.diffuse, fragment_texCoords));
    else
		ambient = light.ambient *  fragment_color;
    // diffuse
	vec3 lightDir = normalize(light.position - fragment_position);
    vec3 norm = normalize(fragment_normal);
    float diff = max(dot(norm, lightDir), 0.0);
        
	vec3 diffuse;
	if(byColor==0)
		diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragment_texCoords));  
    else
		diffuse = light.diffuse * diff *  fragment_color;
        
    // specular
    vec3 viewDir = normalize(view_position - fragment_position);
    vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;  
        
    // attenuation
    float distance    = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

	// calculate shadow
	float shadow = ShadowCalculation(fs_in.fragment_position, light, shadowMap, far_plane);      
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
            
    return lighting;
}

float ShadowCalculation(vec3 fragPos, Light light, samplerCube shadowMap, float far_plane)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - light.position;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(shadowMap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	  
    return shadow;
}

