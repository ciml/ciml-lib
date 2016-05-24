TARGET  := clonalg

CPPOBJDIR = cppobjs

CPPSOURCES := $(wildcard *.cpp)
CPPOBJS := $(CPPSOURCES:%.cpp=$(CPPOBJDIR)/%.o)

COBJDIR = cobjs

CSOURCES := $(wildcard *.c)
COBJS := $(CSOURCES:%.c=$(COBJDIR)/%.o)

OBJS = $(CPPOBJS) $(COBJS)

OPT_FLAGS   := -fno-strict-aliasing -O2 
OPT_FLAGS += -fopenmp

INC := -I/opt/AMDAPP/include/
INC += -I/usr/local/cuda/include/
INC += -I/usr/local/cuda-7.5/include/
INC += -I/usr/local/cuda-5.5/include/

LIB := -L/usr/lib64/
LIB += -lOpenCL -fopenmp

CXXFLAGS := -m64 -DUNIX $(WARN_FLAGS) $(OPT_FLAGS) $(INC) 
CFLAGS := -m64 -DUNIX $(WARN_FLAGS) $(OPT_FLAGS) $(INC) 

LDFLAGS := $(LIBRARY_PATH) $(LIB)

.PHONY: clean

$(TARGET): $(CPPOBJDIR) $(COBJDIR) $(CPPOBJS) $(COBJS)
		g++ -fPIC -o $(TARGET) $(CPPOBJS) $(LDFLAGS) 

$(CPPOBJS): $(CPPOBJDIR)/%.o: %.cpp
		@echo "compile $@ $<"
		g++ -fPIC $(CXXFLAGS) -c $< -o $@
        
$(COBJS): $(COBJDIR)/%.o: %.c
		@echo "compile $@ $<"
		gcc -fPIC $(CFLAGS) -c $< -o $@
        
$(CPPOBJDIR):   
		@ mkdir -p $(CPPOBJDIR)
        
$(COBJDIR):     
		@ mkdir -p $(COBJDIR)

clean:
		$(RM) $(TARGET) $(OBJ)
		$(RM) -rf $(CPPOBJDIR)
		$(RM) -rf $(COBJDIR)
