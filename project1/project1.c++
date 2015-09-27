// project1.c++

#include "GLFWController.h"
#include "ModelView.h"
#include "Read.h"

int main(int argc, char* argv[])
{
	float** data = Read::readData();

	//data arrays for the four trend lines
	vec2 blueData[121];
	vec2 redData[121];
	vec2 yellowData[121];
	vec2 greenData[121];

	vec2 axisData[20] =
	{
		{-10, 0}, {130, 0}, //x-axis
		{0, -0.2}, {0, 1.8}, //y-axis
		//8 horizontal reference lines
		{0, 0.2}, {130, 0.2},
		{0, 0.4}, {130, 0.4},
		{0, 0.6}, {130, 0.6},
		{0, 0.8}, {130, 0.8},
		{0, 1.0}, {130, 1.0},
		{0, 1.2}, {130, 1.2},
		{0, 1.4}, {130, 1.4},
		{0, 1.6}, {130, 1.6}
	};

	//data ---> <color>Data
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 121; j++)
		{
			float tempDataPoint = 0;

			switch(i)
			{
				case 0:
					blueData[j][0] = j;
					blueData[j][1] = data[i][j];
					break;
				case 1:
					redData[j][0] = j;
					redData[j][1] = data[i][j];
					break;
				case 2:
					yellowData[j][0] = j;
					yellowData[j][1] = data[i][j];
					break;
				case 3:
					greenData[j][0] = j;
					greenData[j][1] = data[i][j];
					break;
				default:
					std::cout << "What is happening" << std::endl;
					break;
			}
		}
	}

	GLFWController c(argv[0]);
	c.reportVersions(std::cout);

	// TODO: one or more ModelView dynamic allocations, adding
	//       each to the Controller using "c.addModel(...);"
	ModelView* axisMV = new ModelView(axisData, true);
	c.addModel(axisMV);

	ModelView* blueMV = new ModelView(blueData, false);
	c.addModel(blueMV);

	ModelView* redMV = new ModelView(redData, false);
	c.addModel(redMV);

	ModelView* yellowMV = new ModelView(yellowData, false);
	c.addModel(yellowMV);

	ModelView* greenMV = new ModelView(greenData, false);
	c.addModel(greenMV);

	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);
	// Simplest case: Just tell the ModelView we want to see it all:
	ModelView::setMCRegionOfInterest(xyz);

	c.run();

	delete[] data;

	return 0;
}
