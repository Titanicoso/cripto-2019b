all: src

src: app
	cd src; make all;
	mv ./src/app ./

clean:
	cd src; make clean;
	[ -e app ] && rm app

.PHONY: all clean app