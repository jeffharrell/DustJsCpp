main: dustjs.o
	ar rvs dustjs.a dustjs.o

dustjs.o: libdust.h
	g++ -c dustjs.cpp

libdust.h:
	cd lib/dustjs/ && xxd -i lib/dust.js ../../libdust.h

libcompiler.h:
	cd lib/dustjs/ && xxd -i lib/compiler.js ../../libcompiler.h

clean:
	rm dustjs.a
	rm dustjs.o

dependencies:
	$(MAKE) -C lib/v8 dependencies && $(MAKE) -C lib/v8 x64 -j 8