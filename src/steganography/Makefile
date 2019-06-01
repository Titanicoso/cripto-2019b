include Makefile.inc
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
DEPS = $(wildcard ../*/include/*.h)
DEPSFOLDER = $(wildcard ../*/include)
INC_PARAMS=$(DEPSFOLDER:%=-I%)

fromzero: clean all

all: $(OBJECTS)

%.o: %.c $(DEPS)
	$(GCC) $(GCCFLAGS) $(INC_PARAMS) -c $< 


clean: 
	rm -rf *.o

.PHONY: all clean fromzero