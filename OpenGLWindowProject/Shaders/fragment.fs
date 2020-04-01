#version 330 core

uniform struct Material
{
	sampler2D texture;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shininess;
} material;

uniform struct PointLight
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 attenuation;
} light;

in Vertex{
	vec2 texcoord;
	vec3 normal;
	vec3 lightDir;
	vec3 viewDir;
	float distance;
} vertex;

out vec4 color;

void main(void)
{
	vec3 normal   = normalize(vertex.normal);
	vec3 lightDir = normalize(vertex.lightDir);	
	vec3 viewDir  = normalize(vertex.viewDir);

	float attenuation = 1.0 / (  light.attenuation[0] + 
			             light.attenuation[1] * vertex.distance +
				     light.attenuation[2] * vertex.distance * vertex.distance);

	color = material.emission;
	color += material.ambient * light.ambient * attenuation;
	float NdotL = max(dot(normal, lightDir), 0.0);
	color += material.diffuse * light.diffuse * NdotL * attenuation;
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal) , viewDir), material.shininess), 0.0);
	color += material.specular * light.specular * RdotVpow * attenuation;
	color *= texture(material.texture, vertex.texcoord);
}