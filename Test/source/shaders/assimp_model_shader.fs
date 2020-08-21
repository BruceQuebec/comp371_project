#version 330 core
out vec4 color;

in vec3 fragment_position;
in vec3 fragment_normal;
in vec2 fragment_texCoords;
in vec4 shadow_map_pos;

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
uniform bool render_shadow;
uniform sampler2DShadow shadow_map;



vec3 pointlLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec2 fragment_texCoords, vec3 view_position, bool useTexture, vec4 shadow_map_pos, bool render_shadow);

void main()
{
	//color = vec4(1.0);
	//color = texture(material.texture_diffuse1, fragment_texCoords);
	color = vec4(pointlLightRender(light, material, fragment_normal, fragment_position, fragment_texCoords, view_position, useTexture, shadow_map_pos, render_shadow),1.0);
}

vec3 pointlLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec2 fragment_texCoords, vec3 view_position, bool useTexture, vec4 shadow_map_pos, bool render_shadow){
	vec3 norm = normalize(fragmemt_normal);
    vec3 lightDir = normalize(light.position - fragment_position);

	//shadow
	vec3 shadow_pos = shadow_map_pos.xyz/shadow_map_pos.w;

	float bias = max(0.5 * (1.0 - dot(normalize(norm), normalize(lightDir))), 0.05);

	float z = shadow_map_pos.z - bias;

	shadow_pos = vec3(shadow_map_pos.x/shadow_map_pos.w, shadow_map_pos.y/shadow_map_pos.w, z/shadow_map_pos.w);

	shadow_pos = shadow_pos * 0.5 + 0.5;
	
	vec3 texture_color = useTexture ? vec3(texture(material.texture_diffuse1, fragment_texCoords).rgb): material.diffuse.rgb;
	vec3 ambient_color = texture_color;

	float cos_theta = max(dot(norm, lightDir), 0);
	vec3 diffuse_color = cos_theta * texture_color;

	vec3 viewDir = normalize(view_position - fragment_position);
	vec3 reflect_direction = reflect(-normalize(lightDir), norm);
	float cos_alpha = max(dot(normalize(viewDir), normalize(reflect_direction)), 0);
	vec3 specular_color = pow(cos_alpha, 1) * texture_color;

	float distance = length(light.position - fragment_position);

	float normalized_distance = max((200 - distance) / 200, 0);

	vec3 color;
	if (render_shadow)
		color = (material.ambient.rgb * ambient_color + vec3(texture(shadow_map, shadow_pos)) * light.ambient * material.diffuse.rgb * diffuse_color + vec3(texture(shadow_map, shadow_pos)) * light.ambient * material.specular.rgb * specular_color) * normalized_distance;
	else
		color = (material.ambient.rgb * ambient_color + light.ambient * material.specular.rgb * specular_color) * normalized_distance;
	
	// ambient
	//vec3 color_base = useTexture ? vec3(texture(material.texture_diffuse1, fragment_texCoords).rgb): material.diffuse.rgb;
	
	//vec3 color_base = vec3(texture(material.texture_diffuse1, fragment_texCoords));
    
	//vec3 ambient = light.ambient * color_base;
  	
    // diffuse 
    //vec3 norm = normalize(fragmemt_normal);
    //vec3 lightDir = normalize(light.position - fragment_position);
	
    //float diff = max(dot(norm, lightDir), 0.0);
   // vec3 diffuse = light.diffuse * diff * color_base;  
      
    // attenuation
    //float distance    = length(light.position - fragment_position);
    //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
	// specular
	//vec3 specular_base = useTexture ? vec3(texture(material.texture_specular1, fragment_texCoords).rgb): material.specular.rgb;
    //vec3 viewDir = normalize(view_position - fragment_position);
    //vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 96.078431);
    //vec3 specular = light.specular * spec *  specular_base ;
	
    //ambient  *= attenuation;  
    //diffuse   *= attenuation; 
    //specular *= attenuation; 

	

	//vec3 result;
	//if(render_shadow){
		//result = ambient*texture(shadow_map, shadow_pos) +  diffuse*texture(shadow_map, shadow_pos) + specular*texture(shadow_map, shadow_pos);
	//}
	//else{
	//	result = ambient +  diffuse + specular;
	//}
    
    return color;
}
