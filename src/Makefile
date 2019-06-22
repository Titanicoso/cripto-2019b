include Makefile.inc
OBJECTS = $(wildcard ./*/*.o)
DEPS = $(wildcard ./*/include/*.h)
DEPSFOLDER = $(wildcard ../*/include)
INC_PARAMS=$(DEPSFOLDER:%=-I%)

all: main matrix bmp random steganography ss 

fromzero: clean all ; ./ss -d -s "../try/Secreto.bmp" -m "../try/Marca.bmp" -k 4 -n 8 -i "../try/shares"

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

ss:
	$(MAIN): $(DEPS) $(OBJECTS)
	$(GCC) $(OBJECTS) -o $@  $(INC_PARAMS) $(GCCFLAGS)

clean:
	cd main; make clean
	cd matrix; make clean
	cd steganography; make clean
	cd random; make clean
	cd bmp; make clean

.PHONY: all clean fromzero bmp random main steganography matrix ss