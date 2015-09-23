#version 410 core

in vec2 mcPosition;
in float vertexFraction;
in vec3 vertexColor;

uniform vec4 scaleTrans;
out vec3 colorToFS;
out float fractionToFS;

void main()
{
	colorToFS = vertexColor;
	fractionToFS = vertexFraction;
	float ldsX = scaleTrans[0]*mcPosition.x + scaleTrans[1];
	float ldsY = scaleTrans[2]*mcPosition.y + scaleTrans[3];
	gl_Position = vec4(ldsX, ldsY, 0, 1);
}
