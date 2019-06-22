all: src

src: ss
	cd src; make all;
	mv ./src/ss ./

clean:
	cd src; make clean;
	rm -f ss

.PHONY: all clean app ss