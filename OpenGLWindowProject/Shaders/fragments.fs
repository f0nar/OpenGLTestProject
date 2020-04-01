#version 330 core
uniform sampler2D colorTexture;
in vec2 fragTexcoord;
out vec4 color;
void main(void)
{
	color = texture(colorTexture, fragTexcoord);
	//color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}