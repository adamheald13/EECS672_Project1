// twoTriangles.c++ (V1): Adding client-side specification of color via uniform variables

#include "GLFWController.h"
#include "ModelView.h"

int main(int argc, char* argv[])
{
	GLFWController c("twoTriangles: V1");
	c.reportVersions(std::cout);
	c.addModel( new ModelView() );

	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);
	// Simplest case: Just tell the ModelView we want to see it all:
	ModelView::setMCRegionOfInterest(xyz);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	c.run();

	return 0;
}
