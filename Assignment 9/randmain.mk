all: randcpuid.o, randlibhw.so, randlibsw.so, randmain

randcpuid.o:		randcpuid.c
	$(CC) $(CFLAGS) -c randcpuid.c

randlibhw.so:		randlibhw.c
	$(CC) $(CFLAGS) randlibhw.c -shared -fPIC -o randlibhw.so

randlibsw.so:		randlibsw.c
	$(CC) $(CFLAGS) randlibsw.c -shared -fPIC -o randlibsw.so

randmain:		randcpuid.o
	$(CC) $(CFLAGS) randmain.c randcpuid.o -ldl -Wl,-rpath=$(PWD) -o randmain
