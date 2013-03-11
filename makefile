CC=g++
CFLAGS=-Wall
INCLUDES=-Ilib/v8/include
LIBS=lib/v8/out/x64.release/libv8_base.a lib/v8/out/x64.release/libv8_snapshot.a

main:
	$(CC) $(CFLAGS) $(INCLUDES) ./dustjs.cpp -o ./out/dustjs $(LIBS) -lpthread

clean:
	rm -rf out/dustjs

dependencies:
	$(MAKE) -C lib/v8 dependencies && $(MAKE) -C lib/v8 x64 -j 8