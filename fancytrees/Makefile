CPP = g++
INC = -I../glslutil -I../mvcutil -I.
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = ../lib/libglsl.so
OGL_LIBRARIES = -lglfw -lGLU -lGL

OBJS = main.o ModelView.o Controller.o GLFWController.o

main: $(OBJS) ../lib/libglsl.so
	$(LINK) -o main $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(OGL_LIBRARIES)

../lib/libglsl.so : ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

main.o: main.c++
	$(CPP) $(C_FLAGS) main.c++
ModelView.o: ModelView.h ModelView.c++
	$(CPP) $(C_FLAGS) ModelView.c++
Controller.o: ../mvcutil/Controller.h ../mvcutil/Controller.c++
	$(CPP) $(C_FLAGS) ../mvcutil/Controller.c++
GLFWController.o: ../mvcutil/GLFWController.h ../mvcutil/GLFWController.c++
	$(CPP) $(C_FLAGS) ../mvcutil/GLFWController.c++
