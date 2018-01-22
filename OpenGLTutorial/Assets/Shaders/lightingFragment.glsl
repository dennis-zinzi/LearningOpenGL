#version 330 core

#define NUM_POINT_LIGHTS 4

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};


struct DirLight{
	vec3 dir;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 pos;
	vec3 dir;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 myNormal;
in vec3 fragPos;
in vec2 texCoords;


out vec4 color;

uniform vec3 viewPos;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLight;

//Func declaration
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fgPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fgPos, vec3 viewDir);

//Func definition
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.dir);
	float diff = max(dot(normal, lightDir), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	return ambient + diffuse + specular;
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fgPos, vec3 viewDir){
	vec3 lightDir = normalize(light.pos - fgPos);
	float diff = max(dot(normal, lightDir), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	float dist = length(light.pos - fgPos);
	float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist*dist));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fgPos, vec3 viewDir){
	vec3 lightDir = normalize(light.pos - fgPos);
	float diff = max(dot(normal, lightDir), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	float dist = length(light.pos - fgPos);
	float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist*dist));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	float theta = dot(lightDir, normalize(-light.dir));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);


	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}



void main(){

	vec3 norm = normalize(myNormal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	for(int i = 0; i < NUM_POINT_LIGHTS; i++){
		result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
	}

	result += CalcSpotLight(spotLight, norm, fragPos, viewDir);

	color = vec4(result, 1.0f);
	////Get diffuse tex
	//vec3 tex = vec3(texture(material.diffuse, texCoords));
	
	////Ambient
	//vec3 ambient = light.ambient * tex;

	////Diffuse
	//vec3 norm = normalize(myNormal);
	//vec3 lightDir = normalize(light.pos - fragPos);
	////vec3 lightDir = normalize(-light.dir); //Directional lighting
	//float diff = max(dot(norm, lightDir), 0.0f);
	//vec3 diffuse = light.diffuse * diff * tex;

	////Specular
	//vec3 viewDir = normalize(viewPos - fragPos);
	//vec3 reflectDir = reflect(-lightDir, norm);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	//vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	////Spotlight
	//float theta = dot(lightDir, normalize(-light.dir));
	//float epsilon = light.cutOff - light.outerCutOff;
	//float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	//diffuse *= intensity;
	//specular *= intensity;

	////Attenuation
	//float dist = length(light.pos - fragPos);
	//float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist*dist));

	//ambient *= attenuation;
	//diffuse *= attenuation;
	//specular *= attenuation;

	//vec3 lightTot = ambient + diffuse + specular;


	//color = vec4(lightTot, 1.0f);
}
