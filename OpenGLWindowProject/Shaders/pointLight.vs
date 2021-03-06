#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
uniform struct Transform
{
	mat4 model;
	mat4 view;
	mat4 projection;
	mat3 normal;
	vec3 viewPos;
} transform;

uniform struct PointLight
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 attenuation;
} light;
out Vertex{
	vec2 texcoord;
	vec3 normal;
	vec3 lightDir;
	vec3 viewDir;
	float distance;
} vertex;

void main(void)
{
	vec4 vert = transform.model * vec4(position, 1.0);
	vec4 lightDir = light.position - vert;

	vertex.texcoord = texcoord;
	vertex.normal = transform.normal * normal;
	vertex.lightDir = vec3(lightDir);
	vertex.viewDir = transform.viewPos - vec3(vert);
	vertex.distance = length(lightDir);

	mat4 viewProjection = transform.projection * transform.view;
	gl_Position = viewProjection * vert;
}
