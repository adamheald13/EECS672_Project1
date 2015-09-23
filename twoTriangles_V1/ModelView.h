// ModelView.h - a basic combined Model and View for OpenGL

#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <string>

#ifdef __APPLE_CC__
#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

class ShaderIF;

// for interfacing to common GLSL data types
typedef float vec3[3]; 

class ModelView
{
public:
	ModelView();
	virtual ~ModelView();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	void render() const;

	static void setMCRegionOfInterest(double xyz[]);
private:
	GLuint vao[2]; // we will create two individual objects
	GLuint vbo[2]; // each object has its own coordinate buffer ...
	vec3 useColor[2]; // ... and its own color information

	// interactive display options
	bool drawInBlack[2]; // i.e., instead of "useColor"
	bool visible[2]; // i.e., whether we actually currently draw it

	// we assume all instances share the same shader program:
	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	// "pvaLoc_" prefix means shader program location of an "attribute" variable
	// "ppuLoc_" prefix means shader program location of an "uniform" variable
	static GLint ppuLoc_color; // a uniform variable (per-primitive)
	static GLint ppuLoc_scaleTrans; // a uniform used to scale MC to LDS
	static GLint pvaLoc_mcPosition; // an attribute variable (per-vertex)

	// "pp": "per-primitive"; "pv": "per-vertex"
	static GLint ppUniformLocation(GLuint glslProgram, const std::string& name);
	static GLint pvAttribLocation(GLuint glslProgram, const std::string& name);

	// Routines for computing parameters necessary to map from arbitrary
	// model coordinate ranges into OpenGL's -1..+1 Logical Device Space.
	// 1. linearMap determines the scale and translate parameters needed in
	//    order to map a value, f (fromMin <= f <= fromMax) to its corresponding
	//    value, t (toMin <= t <= toMax). Specifically: t = scale*f + trans.
	static void linearMap(double fromMin, double fromMax,
		double toMin, double toMax, double& scale, double& trans);
	// 2. compute2DScaleTrans uses the current model coordinate region of
	//    interest along with linearMap to determine how to map coordinates
	//    in the region of interest to their proper location in Logical Device
	//    Space.
	//    (Returns float[] because glUniform currently allows only float[].)
	static void compute2DScaleTrans(float* scaleTrans);

	// Current MC region of interest
	static double mcRegionOfInterest[6];

	void initModelGeometry();
	void deleteObject(int which);

	static void fetchGLSLVariableLocations();
};

#endif
