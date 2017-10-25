CPPFLAGS=-Wall -Wextra -pedantic -std=c++1z -O2 -I Header -I ../OpenGLEngine/Header

SRCS=$(shell find . | grep "cpp")
OBJS=$(subst .cpp,.o,$(SRCS))

astroidExplorer.a : $(OBJS)
	ar crvs astroidExplorer.a $(OBJS)

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) astroidExplorer.a 
	$(RM) $(OBJS)
	$(RM) .depend

include .depend

