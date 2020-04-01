#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;
out vec2 fragTexcoord;
void main(void)
{
	// перевод позиции вершины из локальных координат в однородные
	mat4 modelView = u_viewMatrix * u_modelMatrix;
	mat4 modelViewProjection = u_projectionMatrix * modelView;
	gl_Position = modelViewProjection * vec4(position, 1.0f);
	// передадим текстурные координаты в фрагментный шейдер
	fragTexcoord = texcoord;
}
