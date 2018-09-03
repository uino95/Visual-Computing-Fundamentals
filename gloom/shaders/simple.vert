#version 330 core

layout(location =0) in vec3 position;

void main()
{
	// Mirror the position respect the x and y-axis

	// mat3x3 mirror;
	// mirror[0] = vec3(-1.0f, 0.0f, 0.0f);
	// mirror[1] = vec3(0.0f, -1.0f, 0.0f);
	// mirror[2] = vec3(0.0f, 0.0f, 0.0f);
	// vec3 mirrored_position = mirror * position;
    // gl_Position = vec4(mirrored_position, 1.0f);

    gl_Position = vec4(position, 1.0f);
}
