#version 330 core

out vec4 color;

void main()
{
	// Add checkerboard texture
	if((int(gl_FragCoord.x) % 100) < 50 && (int(gl_FragCoord.y) % 100) < 50){
		color = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	} else if((int(gl_FragCoord.y) % 100) > 50 &&  (int(gl_FragCoord.x) % 100) > 50){
		color = vec4(0.0f, 1.0f, 0.0f, 0.0f);	
	} else {
		color = vec4(0.0f, 0.0f, 1.0f, 0.0f);
	}    
}
