CPPFLAGS=-Wall -Wextra -pedantic -std=c++1z -O2 -I Header 

SRCS=$(shell find . | grep "cpp")
OBJS=$(subst .cpp,.o,$(SRCS))


openGLEngine.a : $(OBJS)
	ar crvs openGLEngine.a $(OBJS)

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) openGLEngine.a 
	$(RM) $(OBJS)
	$(RM) .depend

include .depend

