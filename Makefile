CXX ?= $(CROSS_COMPILE)g++

LIB_OEM_OBJ = oemhandler.o
LIB_OEM     = liboemhandler.so

INC_FLAG += -I. -O2 --std=gnu++11
LIB_FLAG += -rdynamic


all: $(LIB_OEM)

%.o: %.C
	$(CXX) -fpic -c $< $(CXXFLAGS) $(INC_FLAG) $(IPMID_PATH) -o $@
	

$(LIB_OEM): $(LIB_OEM_OBJ)
	$(CXX) $^ -shared $(LDFLAGS) $(LIB_FLAG) -o $@


clean:
	rm -f *.o *.so
