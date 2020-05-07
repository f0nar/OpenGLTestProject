#version 410 core

layout(location = 0) in vec4 sphData;

uniform struct Transform
{
	mat4 model;
	mat4 view;
	mat4 projection;
	mat3 normal;
	vec3 viewPos;
} transform;

out SphereData
{
    vec3 center;
    float radius;
} sphereData;

void main(void)
{
    sphereData.center = sphData.xyz;
    sphereData.radius = sphData.w;

    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}