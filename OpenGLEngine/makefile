CXXFLAGS=-Wall -Wextra -pedantic -std=c++1z -O2 -I Header

SRCS=$(shell find . | grep "cpp")
OBJS=$(subst .cpp,.o,$(SRCS))


openGLEngine.a : $(OBJS)
	ar crvs $@ $(OBJS)

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CXXFLAGS) -MM $^ >> ./.depend;

clean:
	$(RM) openGLEngine.a 
	$(RM) $(OBJS)
	$(RM) .depend

include .depend

