#version 330 core

in vec3 fragment_position;
in vec3 fragment_normal;
in vec2 fragment_texCoords;

struct Material {
	vec3 ambient;
	vec3 specular;
	sampler2D diffuse;

	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

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


out vec4 color;
vec3 normalLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec3 view_position);
vec3 pointLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec3 view_position);
vec3 spotLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec3 view_position);

void main()
{
	vec3 result = normalLightRender(light, material, fragment_normal, fragment_position, view_position);
	color = vec4(result, 1.0);
}

vec3 normalLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec3 view_position){
	// ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragment_texCoords));
  	
    // diffuse 
    vec3 norm = normalize(fragmemt_normal);
    vec3 lightDir = normalize(light.position - fragment_position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, fragment_texCoords)));
    
    // specular
    vec3 viewDir = normalize(view_position - fragment_position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 pointlLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec3 view_position){
	// ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragment_texCoords));
  	
    // diffuse 
    vec3 norm = normalize(fragmemt_normal);
    vec3 lightDir = normalize(light.position - fragment_position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, fragment_texCoords)));
    
    // specular
    vec3 viewDir = normalize(view_position - fragment_position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular); 
	
	float distance    = length(light.position - fragment_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	vec3 result = ambient + diffuse + specular;
    return result;
}
vec3 spotLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec3 view_position){
	vec3 lightDir = normalize(light.position - fragment_position);
    vec3 SpotDir = normalize(light.position - vec3(0,0,-1));
	vec3 result;
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, SpotDir); 
	float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    if(theta > light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {   
        // ambient
        vec3 ambient = light.ambient *  vec3(texture(material.diffuse, fragment_texCoords));
        
        // diffuse 
        vec3 norm = normalize(fragmemt_normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragment_texCoords)) * intensity;  
		
        
        // specular
        vec3 viewDir = normalize(view_position - fragment_position);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * material.specular * intensity;  
        
        // attenuation
        float distance    = length(light.position - fragment_position);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        diffuse *= attenuation;
        specular *= attenuation;   
            
        result = (ambient + diffuse + specular);
	}
	else
	{
		result = vec3(texture(material.diffuse, fragment_texCoords)*0.1);
	}
		
	return result;
}
