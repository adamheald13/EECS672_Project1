CPP = g++
INC = -I../glslutil -I../mvcutil -I.
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = ../lib/libglsl.so
OGL_LIBRARIES = -lglfw -lGLU -lGL

OBJS = project1.o ModelView.o Controller.o GLFWController.o Read.o

project1: $(OBJS) ../lib/libglsl.so
	$(LINK) -o project1 $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(OGL_LIBRARIES)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

project1.o: project1.c++
	$(CPP) $(C_FLAGS) project1.c++
ModelView.o: ModelView.h ModelView.c++
	$(CPP) $(C_FLAGS) ModelView.c++
Read.o: Read.h Read.c++
	$(CPP) $(C_FLAGS) Read.c++
Controller.o: ../mvcutil/Controller.h ../mvcutil/Controller.c++
	$(CPP) $(C_FLAGS) ../mvcutil/Controller.c++
GLFWController.o: ../mvcutil/GLFWController.h ../mvcutil/GLFWController.c++
	$(CPP) $(C_FLAGS) ../mvcutil/GLFWController.c++
clean: rm *.o project1
