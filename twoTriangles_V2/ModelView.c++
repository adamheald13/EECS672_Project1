// ModelView.c++ - a basic combined Model and View for OpenGL

#include <iostream>

#include "ModelView.h"
#include "ShaderIF.h"

ShaderIF* ModelView::shaderIF = NULL;
int ModelView::numInstances = 0;
GLuint ModelView::shaderProgram = 0;
GLint ModelView::ppuLoc_colorMode = -2; // uniform variable (per-primitive)
GLint ModelView::ppuLoc_scaleTrans = -2;
GLint ModelView::pvaLoc_mcPosition = -2; // attribute variable (per-vertex)

// Current MC Region of interest
double ModelView::mcRegionOfInterest[6] = { -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 };

static const int numVerticesInTriangle = 3;

ModelView::ModelView(vec2* triangleVertices) : colorMode(8), visible(true)
{
	if (ModelView::shaderProgram == 0)
	{
		// Create the common shader program:
		ModelView::shaderIF = new ShaderIF("twoTriangles_V2.vsh", "twoTriangles_V2.fsh");
		ModelView::shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}

	initModelGeometry(triangleVertices);
	ModelView::numInstances++;
}

ModelView::~ModelView()
{
	// delete the vertex array objects and buffers, if they have not
	// already been deleted
	deleteObject();
	if (--numInstances == 0)
	{
		ModelView::shaderIF->destroy();
		delete ModelView::shaderIF;
		ModelView::shaderIF = NULL;
		ModelView::shaderProgram = 0;
	}
}

void ModelView::compute2DScaleTrans(float* scaleTransF) // CLASS METHOD
{
	// We are only concerned with the xy extents for now
	// Map the desired limits to the -1..+1 Logical Device Space:
	double scaleTrans[4];
	linearMap(mcRegionOfInterest[0], mcRegionOfInterest[1], -1.0, 1.0,
		scaleTrans[0], scaleTrans[1]);
	linearMap(mcRegionOfInterest[2], mcRegionOfInterest[3], -1.0, 1.0,
		scaleTrans[2], scaleTrans[3]);
	for (int i=0 ; i<4 ; i++)
		scaleTransF[i] = static_cast<float>(scaleTrans[i]);
}

void ModelView::deleteObject()
{
	if (vao[0] > 0) // hasn't already been deleted
	{
		glDeleteBuffers(1, vbo);
		glDeleteVertexArrays(1, vao);
		vao[0] = vbo[0] = 0;
	}
}

void ModelView::fetchGLSLVariableLocations()
{
	if (ModelView::shaderProgram > 0)
	{
		ModelView::ppuLoc_colorMode = ppUniformLocation(shaderProgram, "colorMode");
		ModelView::ppuLoc_scaleTrans = ppUniformLocation(shaderProgram, "scaleTrans");
		// "mc" in "mcPosition" refers to "model coordinates"
		ModelView::pvaLoc_mcPosition = pvAttribLocation(shaderProgram, "mcPosition");
	}
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void ModelView::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin; xyzLimits[1] = xmax;
	xyzLimits[2] = ymin; xyzLimits[3] = ymax;
	xyzLimits[4] = -1.0; xyzLimits[5] =  1.0; // (zmin, zmax) (really 0..0)
}

void ModelView::handleCommand(unsigned char key, double ldsX, double ldsY)
{
	// determine if this triangle was "picked" based on whether the
	// given location is inside the bounding box of this triangle.
	float st[4];
	compute2DScaleTrans(st);
	float xMC = (ldsX - st[1]) / st[0];
	float yMC = (ldsY - st[3]) / st[2];
	if ((xMC < xmin) || (xMC > xmax) || (yMC < ymin) || (yMC > ymax))
		return;

	// Yes - this triangle: Now apply the current operation to it:
	if (key == 'v')
		// toggle visibility
		visible = !visible;
	else if (key == 'd')
		deleteObject();
	else if ((key >= '0') && (key <= '9'))
	{
		// set a new color mode to be used in the fragment shader
		int requestedNewMode = static_cast<int>(key) - static_cast<int>('0');
		colorMode = requestedNewMode;
	}
}

void ModelView::initModelGeometry(vec2* vertices) // assume number of vertices == 3
{
	// Create the VAO and vertex buffer names
	glGenVertexArrays(1, vao);
	glGenBuffers(1, vbo);

	// (initialize and) make the i-th set active
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	// Allocate space for and send data to GPU
	int numBytesInBuffer = numVerticesInTriangle * sizeof(vec2);
	glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(ModelView::pvaLoc_mcPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(ModelView::pvaLoc_mcPosition);

	// make note of the min/max coordinates
	xmin = xmax = vertices[0][0];
	ymin = ymax = vertices[0][1];
	for (int i=1 ; i<3 ; i++)
	{
		if (vertices[i][0] < xmin)
			xmin = vertices[i][0];
		else if (vertices[i][0] > xmax)
			xmax = vertices[i][0];
		if (vertices[i][1] < ymin)
			ymin = vertices[i][1];
		else if (vertices[i][1] > ymax)
			ymax = vertices[i][1];
	}
}

// linearMap determines the scale and translate parameters needed in
// order to map a value, f (fromMin <= f <= fromMax) to its corresponding
// value, t (toMin <= t <= toMax). Specifically: t = scale*f + trans.
void ModelView::linearMap(double fromMin, double fromMax, double toMin, double toMax,
					  double& scale, double& trans) // CLASS METHOD
{
	scale = (toMax - toMin) / (fromMax - fromMin);
	trans = toMin - scale*fromMin;
}

GLint ModelView::ppUniformLocation(GLuint glslProgram, const std::string& name)
{
	GLint loc = glGetUniformLocation(glslProgram, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-primitive uniform: '" << name << "'\n";
	return loc;
}

GLint ModelView::pvAttribLocation(GLuint glslProgram, const std::string& name)
{
	GLint loc = glGetAttribLocation(glslProgram, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-vertex attribute: '" << name << "'\n";
	return loc;
}

void ModelView::render() const
{
	if ((vao[0] == 0)  || // if it has been deleted
	    (!visible))    // if it has been temporarily blanked
		return;

	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);

	// draw the triangles using our vertex and fragment shaders
	glUseProgram(shaderProgram);

	// define the mapping from MC to -1..+1 Logical Device Space:
	float scaleTrans[4];
	compute2DScaleTrans(scaleTrans);
	glUniform4fv(ModelView::ppuLoc_scaleTrans, 1, scaleTrans);

	// establish the current color mode and draw
	glUniform1i(ModelView::ppuLoc_colorMode, colorMode);

	// bind the current triangle VAO
	glBindVertexArray(vao[0]);

	// Draw the triangle.
	glDrawArrays(GL_TRIANGLES, 0, numVerticesInTriangle);

	// restore the previous program
	glUseProgram(pgm);
}

void ModelView::setMCRegionOfInterest(double xyz[6])
{
	for (int i=0 ; i<6 ; i++)
		mcRegionOfInterest[i] = xyz[i];
}
