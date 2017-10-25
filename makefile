CPPFLAGS=-Wall -Wextra -pedantic -std=c++1z -O2 -I AstroidExplorer/Header -I OpenGLEngine/Header
LDFLAGS=-lGL -lGLEW -lglfw -lX11 -lpthread -lXrandr -lSOIL 

LIBS=AstroidExplorer/astroidExplorer.a OpenGLEngine/openGLEngine.a 

astroid: $(LIBS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o astroid $(LIBS)

OpenGLEngine/openGLEngine.a :
	cd OpenGLEngine && $(MAKE)

AstroidExplorer/astroidExplorer.a :
	cd AstroidExplorer && $(MAKE)

clean:
	$(RM) astroid
	cd OpenGLEngine && $(MAKE) clean
	cd AstroidExplorer && $(MAKE) clean

