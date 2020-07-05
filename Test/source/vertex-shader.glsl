// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

// Values that stay constant for the whole mesh.
uniform mat4 mvpMat;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  mvpMat * vec4(vertexPosition, 1);
	fragmentColor = vertexColor;
}