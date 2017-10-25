CPPFLAGS=-Wall -Wextra -pedantic -std=c++1z -O2 -I AstroidExplorer/Header -I OpenGLEngine/Header
LDFLAGS=-lGL -lGLEW -lglfw -lX11 -lpthread -lXrandr -lSOIL

SRCS=./OpenGLEngine/Source/Mesh.cpp ./OpenGLEngine/Source/OpenGLEngine.cpp ./OpenGLEngine/Source/Shader.cpp ./OpenGLEngine/Source/Camera.cpp ./OpenGLEngine/Source/Stock.cpp ./OpenGLEngine/Source/System.cpp ./AstroidExplorer/Source/Entities.cpp ./AstroidExplorer/Source/AerodynamicComponent.cpp ./AstroidExplorer/Source/Systems.cpp ./AstroidExplorer/Source/main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

astroid: $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o astroid $(OBJS)

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)
	$(RM) .depend

include .depend

