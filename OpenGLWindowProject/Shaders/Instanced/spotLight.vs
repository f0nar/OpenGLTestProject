#version 410 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texcoord;
layout(location = 1) in vec3 normal;
layout(location = 3) in mat4 model;

// параметры преобразований
uniform struct Transform
{
	mat4 view;
	mat4 projection;
	mat4 globalModel;
	vec3 viewPos;
} transform;

// параметры источника освещения
uniform struct Light
{
	vec4  ambient;
	vec4  diffuse;
	vec4  specular;
	vec4  position;
	vec3  attenuation;
	vec3  spotDirection;
	float spotExponent;
	float spotCosCutoff;
} light;

// параметры для фрагментного шейдера
out Vertex
{
	vec2  texcoord;
	vec3  normal;
	vec3  lightDir;
	vec3  viewDir;
	float distance;
} vertex;

void main(void)
{
	mat4 modelM = model * transform.globalModel; //transform.globalModel * transform.model[gl_InstanceID];
	vec4 vert = modelM * vec4(position, 1.0);
	mat3 normalM = transpose(inverse(mat3(modelM)));

	vec4 lightDir = light.position - vert;

	// передадим в фрагментный шейдер некоторые параметры
	// передаем текстурные координаты
	vertex.texcoord = texcoord;
	// передаем нормаль в мировой системе координат
	vertex.normal   = normalM * normal; //transform.normal * normal;
	// передаем направление на источник освещения
	vertex.lightDir = vec3(lightDir);
	// передаем направление от вершины к наблюдателю в мировой системе координат
	vertex.viewDir  = transform.viewPos - vec3(vert);
	// передаем рассятоние от вершины до истчоника освещения
	vertex.distance = length(lightDir);

	// переводим координаты вершины в однородные
	gl_Position = transform.projection * transform.view * vert;
}
