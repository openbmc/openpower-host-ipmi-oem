CXX ?= $(CROSS_COMPILE)g++

TESTER      = testit
TESTER_OBJ  = testit.o oemhandler.o

LIB_OEM_OBJ = oemhandler.o
LIB_OEM     = liboemhandler.so

INC_FLAG += -I. -O2 --std=gnu++11
LIB_FLAG += -rdynamic


all:  $(TESTER) $(LIB_OEM)

%.o: %.C
	$(CXX) -fpic -c $< $(CXXFLAGS) $(INC_FLAG) $(IPMID_PATH) -o $@
	

$(LIB_OEM): $(LIB_OEM_OBJ)
	$(CXX) $^ -shared $(LDFLAGS) $(LIB_FLAG) -o $@


$(TESTER): $(TESTER_OBJ)
	$(CXX) $^ $(LDFLAGS) $(LIB_FLAG) -o $@ -ldl

clean:
	rm -f $(TESTER) *.o *.so
