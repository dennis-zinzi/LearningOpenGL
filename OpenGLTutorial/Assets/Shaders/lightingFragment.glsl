#version 330 core

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	//vec3 dir;
	vec3 pos;
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
uniform Light light;


void main(){
	//Get diffuse tex
	vec3 tex = vec3(texture(material.diffuse, texCoords));
	
	//Ambient
	vec3 ambient = light.ambient * tex;

	//Diffuse
	vec3 norm = normalize(myNormal);
	vec3 lightDir = normalize(light.pos - fragPos);
	//vec3 lightDir = normalize(-light.dir); //Directional lighting
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * tex;

	//Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	//Attenuation
	float dist = length(light.pos - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist*dist));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 lightTot = ambient + diffuse + specular;


	color = vec4(lightTot, 1.0f);
	//lightColor * objColor;
}
