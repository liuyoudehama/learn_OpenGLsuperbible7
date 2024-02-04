#version 450 core

layout(location = 0) out vec4 color;

void main(void)
{
	// gl_Position = veccc4(1.0, 0.0, 0.0, 1.0); // Output a red pixel
	color = vec4(1.0, 0.0, 0.0, 1.0);
}