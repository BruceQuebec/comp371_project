#version 330 core
out vec4 color;

in vec3 fragment_position;
in vec3 fragment_normal;
in vec2 fragment_texCoords;

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;

	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
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
uniform bool useTexture = false;



vec3 pointlLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec2 fragment_texCoords, vec3 view_position, bool useTexture);

void main()
{
	//color = vec4(1.0);
	//color = texture(material.texture_diffuse1, fragment_texCoords);
	color = vec4(pointlLightRender(light, material, fragment_normal, fragment_position, fragment_texCoords, view_position, useTexture),1.0);
}

vec3 pointlLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec2 fragment_texCoords, vec3 view_position, bool useTexture){
	// ambient
	vec3 color_base = useTexture ? vec3(texture(material.texture_diffuse1, fragment_texCoords).rgb): material.diffuse.rgb;
	
	//vec3 color_base = vec3(texture(material.texture_diffuse1, fragment_texCoords));
    
	vec3 ambient = light.ambient * color_base;
  	
    // diffuse 
    vec3 norm = normalize(fragmemt_normal);
    vec3 lightDir = normalize(light.position - fragment_position);
	
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * color_base;  
      
    // attenuation
    float distance    = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
	// specular
	vec3 specular_base = useTexture ? vec3(texture(material.texture_specular1, fragment_texCoords).rgb): material.specular.rgb;
    vec3 viewDir = normalize(view_position - fragment_position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 96.078431);
    vec3 specular = light.specular * spec *  specular_base ;
	
    ambient  *= attenuation;  
    diffuse   *= attenuation; 
    specular *= attenuation; 
	  
    vec3 result = ambient +  diffuse + specular;
    return result;
}
