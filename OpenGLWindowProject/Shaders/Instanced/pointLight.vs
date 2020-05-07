#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

uniform struct Transform
{
	samplerBuffer model_tbo;
	mat4 view;
	mat4 projection;
	mat4 globalModel;
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
	vec4 col1 = texelFetch(transform.model_tbo, gl_InstanceID * 4);
    vec4 col2 = texelFetch(transform.model_tbo, gl_InstanceID * 4 + 1);
    vec4 col3 = texelFetch(transform.model_tbo, gl_InstanceID * 4 + 2);
    vec4 col4 = texelFetch(transform.model_tbo, gl_InstanceID * 4 + 3);

	mat4 model = transform.globalModel * mat4(col1, col2, col3, col4); 
	vec4 vert = model * vec4(position, 1.0);
	mat3 normalM = transpose(inverse(mat3(model)));

	vec4 lightDir = light.position - vert;

	vertex.texcoord = texcoord;
	vertex.normal = normalM * normal;//transform.normal * normal;
	vertex.lightDir = vec3(lightDir);
	vertex.viewDir = transform.viewPos - vec3(vert);
	vertex.distance = length(lightDir);

	mat4 viewProjection = transform.projection * transform.view;
	gl_Position = viewProjection * vert;
}
