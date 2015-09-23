// project1.c++

#include "GLFWController.h"
#include "ModelView.h"
#include "Read.h"

int main(int argc, char* argv[])
{
	float** data = Read::readData();

	GLFWController c(argv[0]);
	c.reportVersions(std::cout);

	// TODO: one or more ModelView dynamic allocations, adding
	//       each to the Controller using "c.addModel(...);"

	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);
	// Simplest case: Just tell the ModelView we want to see it all:
	ModelView::setMCRegionOfInterest(xyz);

	c.run();

	return 0;
}
