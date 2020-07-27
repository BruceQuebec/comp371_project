#version 330 core
in VS_OUT{
	vec3 fragment_position;
    vec3 fragment_normal;
	vec3 fragment_color;
    vec2 fragment_texCoords;
    vec4 fragment_position_LightSpace;
}fs_in;


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
uniform int byColor = 0;
uniform sampler2D shadowMap;


out vec4 color;

vec3 spotLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec3 view_position);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
	vec3 result = spotLightRender(light, material, fs_in.fragment_normal, fs_in.fragment_position, view_position);
	color = vec4(result, 1.0);
}

vec3 spotLightRender(Light light, Material material, vec3 fragmemt_normal, vec3 fragment_position, vec3 view_position){
	vec3 lightDir = normalize(light.position - fs_in.fragment_position);
    vec3 SpotDir = normalize(light.position - vec3(0,0,0));
	vec3 result;
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, SpotDir); 
	float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    if(theta > light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {   
        // ambient
        vec3 ambient;
		if(byColor==0)
			ambient = light.ambient *  vec3(texture(material.diffuse, fs_in.fragment_texCoords));
        else
			ambient = light.ambient *  fs_in.fragment_color;
        // diffuse 
        vec3 norm = normalize(fs_in.fragment_normal);
        float diff = max(dot(norm, lightDir), 0.0);
        
		vec3 diffuse;
		if(byColor==0)
			diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.fragment_texCoords)) * intensity;  
        else
			diffuse = light.diffuse * diff *  fs_in.fragment_color;
        
        // specular
        vec3 viewDir = normalize(view_position - fs_in.fragment_position);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * material.specular * intensity;  
        
        // attenuation
        float distance    = length(light.position - fs_in.fragment_position);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        diffuse *= attenuation;
        specular *= attenuation;

		// calculate shadow
		float shadow = ShadowCalculation(fs_in.fragment_position_LightSpace);       
		vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
            
        result = lighting;
	}
	else
	{
		if(byColor==0)
			result = vec3(texture(material.diffuse, fs_in.fragment_texCoords)*0.1);  
        else
			result = light.diffuse * fs_in.fragment_color * 0.1;
	}
		
	return result;
}

float ShadowCalculation(vec4 fragPosLightSpace){
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
