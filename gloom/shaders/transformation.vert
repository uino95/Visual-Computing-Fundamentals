#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colorIn;
out vec4 colorOut;
uniform mat4x4 transformMatrix;

void main()
{
	
	vec4 newPosition = vec4(position, 1.0f);
    gl_Position = transformMatrix * newPosition;

    colorOut = colorIn;
}
