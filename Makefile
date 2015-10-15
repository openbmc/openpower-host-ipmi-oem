CXX ?= $(CROSS_COMPILE)g++

TESTER      = testit
TESTER_OBJ  = testit.o oemhandler.o

LIB_OEM_OBJ = oemhandler.o
LIB_OEM     = liboemhandler.so

LDFLAGS += -rdynamic
CXXFLAGS += -fpic -Wall


all:  $(TESTER) $(LIB_OEM)

%.o: %.C
	$(CXX) -fpic -c $< $(CXXFLAGS) -o $@
	

$(LIB_OEM): $(LIB_OEM_OBJ)
	$(CXX) $^ -shared $(LDFLAGS) $(LIB_FLAG) -o $@


$(TESTER): $(TESTER_OBJ)
	$(CXX) $^ $(LDFLAGS) $(LIB_FLAG) -o $@ -ldl

clean:
	$(RM) $(LIB_OEM_OBJ) $(LIB_OEM) $(TESTER_OBJ) $(TESTER)
