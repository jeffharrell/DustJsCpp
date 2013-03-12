main: dustjs.o
	ar rvs dustjs.a dustjs.o

dustjs.o:
	g++ -c dustjs.cpp

clean:
	rm dustjs.a
	rm dustjs.o

dependencies:
	$(MAKE) -C lib/v8 dependencies && $(MAKE) -C lib/v8 x64 -j 8