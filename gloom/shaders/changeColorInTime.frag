#version 330 core

out vec4 color;
uniform vec4 colorTimeOut;

void main()
{
	// Color which change in time
	color = colorTimeOut;
    
}
