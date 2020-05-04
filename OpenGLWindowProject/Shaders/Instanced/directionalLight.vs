#version 410 core
layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texcoord;
layout(location = 1) in vec3 normal;

layout(location = 3) in mat4 model;

// параметры преобразований
uniform struct Transform
{
	samplerBuffer model_tbo;
	mat4 view;
	mat4 projection;
	mat4 globalModel;
	vec3 viewPos;
} transform;

uniform struct Light
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
} light;

out Vertex
{
	vec2 texcoord;
	vec3 normal;
	vec3 lightDir;
	vec3 viewDir;
} vertex;

void main(void)
{

	vec4 col1 = texelFetch(transform.model_tbo, gl_InstanceID * 4);
    vec4 col2 = texelFetch(transform.model_tbo, gl_InstanceID * 4 + 1);
    vec4 col3 = texelFetch(transform.model_tbo, gl_InstanceID * 4 + 2);
    vec4 col4 = texelFetch(transform.model_tbo, gl_InstanceID * 4 + 3);

	mat4 model = transform.globalModel * mat4(col1, col2, col3, col4); //transform.globalModel * transform.model[gl_InstanceID];
	vec4 vert = model * vec4(position, 1.0);
	mat3 normalM = transpose(inverse(mat3(model)));

	// передадим в фрагментный шейдер некоторые параметры
	// передаем текстурные координаты
	vertex.texcoord = texcoord;
	// передаем нормаль в мировой системе координат
	vertex.normal   = normalM * normal; //transform.normal * normal;
	// передаем направление на источник освещения
	vertex.lightDir = vec3(light.position);
	// передаем направление от вершины к наблюдателю в мировой системе координат
	vertex.viewDir  = transform.viewPos - vec3(vert);

	// переводим координаты вершины в однородные
	gl_Position = transform.projection * transform.view * vert;
}
