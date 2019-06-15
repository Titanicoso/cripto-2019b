include Makefile.inc
OBJECTS = $(wildcard ./*/*.o)
DEPS = $(wildcard ./*/include/*.h)
DEPSFOLDER = $(wildcard ../*/include)
INC_PARAMS=$(DEPSFOLDER:%=-I%)

all: main matrix bmp random steganography app 

fromzero: clean all ; ./app

main:
	cd main; make all

bmp:
	cd bmp; make all

matrix:
	cd matrix; make all

steganography:
	cd steganography; make all

random:
	cd random; make all

app:
	$(MAIN): $(DEPS) $(OBJECTS)
	$(GCC) $(OBJECTS) -o $@  $(INC_PARAMS) $(GCCFLAGS)

clean:
	cd main; make clean
	cd matrix; make clean
	cd steganography; make clean
	cd random; make clean
	cd bmp; make clean

.PHONY: all clean fromzero bmp random main steganography matrix app