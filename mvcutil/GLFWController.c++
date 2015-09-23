// GLFWController.c++: a basic GLFWController (in Model-View-GLFWController sense)

#include <stdlib.h>

#include "GLFWController.h"
#include "ModelView.h"

bool GLFWController::glfwInitialized = false;

GLFWController::GLFWController(const std::string& windowTitle, int rcFlags) :
	returnFromRun(false), runWaitsForAnEvent(true),
	lastPixelPosX(0), lastPixelPosY(0)
{
	if (!glfwInitialized)
	{
		glfwInit();
		glfwInitialized = true;
	}

	// First create the window and its Rendering Context (RC)
	createWindowAndRC(windowTitle, rcFlags);
}

GLFWController::~GLFWController()
{
	// IF THIS IS THE LAST CONTROLLER
	{
		glfwTerminate();
		glfwInitialized = false;
	}
}

void GLFWController::charCB(GLFWwindow* window, unsigned int theChar)
{
	if (theChar < 128)
	{
		GLFWController* c = dynamic_cast<GLFWController*>(curController);
		c->handleAsciiChar(
			static_cast<unsigned char>(theChar), c->lastPixelPosX, c->lastPixelPosY);
	}
}

void GLFWController::createWindowAndRC(const std::string& windowTitle, int rcFlags)
{
	// The following calls enforce use of only non-deprecated functionality.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	int minor = 8; // Start AT LEAST one greater than where you really want to start
	theWindow = NULL;
	while ((theWindow == NULL) && (minor > 0))
	{
		minor--;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
		theWindow = glfwCreateWindow(newWindowWidth, newWindowHeight, titleString(windowTitle).c_str(), NULL, NULL);
	}
	if (theWindow == NULL)
	{
		std::cerr << "\n**** COULD NOT CREATE A 4.x RENDERING CONTEXT ****\n\n";
		glfwTerminate();
		glfwInitialized = false;
	}

	glfwMakeContextCurrent(theWindow);
	establishInitialCallbacksForRC();
}

void GLFWController::establishInitialCallbacksForRC()
{
	glfwSetWindowSizeCallback(theWindow, reshapeCB);
	glfwSetCharCallback(theWindow, charCB);
	glfwSetKeyCallback(theWindow, keyboardCB);
	glfwSetCursorPosCallback(theWindow, mouseMotionCB);
}

void GLFWController::handleDisplay()
{
	glfwMakeContextCurrent(theWindow);
	int width, height;
	glfwGetFramebufferSize(theWindow, &width, &height);
	glViewport(0, 0, width, height);

	// clear the frame buffer
	glClear(glClearFlags);

	// draw the collection of models
	int which = 0;
	for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
		(*it)->render();

	glfwSwapBuffers(theWindow);

	checkForErrors(std::cout, "GLFWController::handleDisplay");
}

void GLFWController::keyboardCB(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (curController != NULL)
	{
		GLFWController* theC = dynamic_cast<GLFWController*>(curController);
		if (key == GLFW_KEY_ESCAPE)
			theC->handleAsciiChar(27, theC->lastPixelPosX, theC->lastPixelPosY);
	}
}

void GLFWController::mouseMotionCB(GLFWwindow* window, double x, double y)
{
	if (curController != NULL)
	{
		GLFWController* c = dynamic_cast<GLFWController*>(curController);
		c->lastPixelPosX = static_cast<int>(x + 0.5);
		c->lastPixelPosY = static_cast<int>(y + 0.5);
	}
}

void GLFWController::reportWindowInterfaceVersion(std::ostream& os) const
{
	os << "        GLFW: " << glfwGetVersionString() << '\n';
}

void GLFWController::reshapeCB(GLFWwindow* window, int width, int height)
{
	dynamic_cast<GLFWController*>(curController)->handleReshape(width, height);
}

void GLFWController::run()
{
	while (!glfwWindowShouldClose(theWindow) && !returnFromRun)
	{
		if (runWaitsForAnEvent)
			glfwWaitEvents();
		else
			glfwPollEvents();
		handleDisplay();
	}
	glfwDestroyWindow(theWindow);
	theWindow = NULL;
}

void GLFWController::setWindowTitle(const std::string& title)
{
	glfwSetWindowTitle(theWindow, title.c_str());
}
