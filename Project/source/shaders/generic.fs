#version 330 core
in VS_OUT{
	vec3 fragment_position;
    vec3 fragment_normal;
	vec3 fragment_color;
    vec2 fragment_texCoords;
    vec4 fragment_position_LightSpace;
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
uniform sampler2D shadowMap;


out vec4 color;

vec3 pointLightRender(Light light, Material material, vec3 view_position, int byColor, vec3 fragment_position, vec3 fragment_normal, vec3 fragment_color, vec2 fragment_texCoords, vec4 fragment_position_LightSpace, sampler2D shadowMap);
float ShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap);

void main()
{
	vec3 result = pointLightRender(light, material, view_position, byColor, fs_in.fragment_position, fs_in.fragment_normal, fs_in.fragment_color, fs_in.fragment_texCoords, fs_in.fragment_position_LightSpace, shadowMap);
	color = vec4(result, 1.0);
}

vec3 pointLightRender(Light light, Material material, vec3 view_position, int byColor, vec3 fragment_position, vec3 fragment_normal, vec3 fragment_color, vec2 fragment_texCoords, vec4 fragment_position_LightSpace, sampler2D shadowMap){
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
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;  
        
    // attenuation
    float distance    = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

	// calculate shadow
	float shadow = ShadowCalculation(fragment_position_LightSpace, shadowMap);      
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
            
    result = lighting;
	return result;
}

float ShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap){
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
	

    return shadow;
}
