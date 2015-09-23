#version 410 core

in vec3 colorToFS;
in float fractionToFS;

// colorMode:
//   0 ==> use interpolated vertex color (colorToFS)
//   1 ==> use interpolated height fraction
//   2 ==> use product of two
uniform int colorMode;

out vec4 fragmentColor;

void main()
{
	if (colorMode == 0)
		fragmentColor = vec4(colorToFS,1.0);
	else if (colorMode == 1)
		fragmentColor = vec4(fractionToFS, fractionToFS, fractionToFS, 1.0);
	else // colorMode == 2
		fragmentColor = vec4(fractionToFS*colorToFS, 1.0);
}
