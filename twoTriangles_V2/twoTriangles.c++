// twoTriangles.c++ (V2): Adding client-side specification of color via uniform variables

#include "GLFWController.h"
#include "ModelView.h"

int main(int argc, char* argv[])
{
	// define vertex coordinates for the two triangles
	vec2 vertexPositions[][3] =
	{
		// triangle 1:
		{ { -6.0, 137.0 }, { 2.0, 137.0 }, { -2.0, 145.0 } },
		// triangle 2:
		{ { -6.0, 135.0 }, { 2.0, 135.0 }, { -2.0, 127.0 } }
	};

	std::cout << vertexPositions[0][0] << std::endl;

	GLFWController c(argv[0]);
	c.reportVersions(std::cout);
	c.addModel( new ModelView(vertexPositions[0]) );
	c.addModel( new ModelView(vertexPositions[1]) );

	glClearColor(1.0, 1.0, 1.0, 1.0);

	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);
	// Simplest case: Just tell the ModelView we want to see it all:
	ModelView::setMCRegionOfInterest(xyz);

	c.run();

	return 0;
}
