#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colorIn;
out vec4 colorOut;
uniform mat4x4 transformMatrix;

void main()
{
    gl_Position = transformMatrix * position;

    colorOut = colorIn;
}
