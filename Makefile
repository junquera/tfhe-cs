#the prefix where you installed tfhe
TFHE_PREFIX=/usr/local
C_INCLUDE_PATH=$C_INCLUDE_PATH:$TFHE_PREFIX/include
CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$TFHE_PREFIX/include
LIBRARY_PATH=$LIBRARY_PATH:$TFHE_PREFIX/lib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TFHE_PREFIX/lib

CC=g++
CFLAGS=-Wall -I$(TFHE_PREFIX)/include -L$(TFHE_PREFIX)/lib
TFHE_LIB=-ltfhe-fftw

all: test server client
	echo "Ok!"

arithmetic.o:
	$(CC) $(CFLAGS) -c tfhe-math/arithmetic.cpp -o arithmetic.o

lib:
	mkdir -p lib

libtfhe-math.a: lib arithmetic.o
	ar crf lib/libtfhe-math.a arithmetic.o

test: libtfhe-math.a
	$(CC) $(CFLAGS) test.cpp reg2.cpp reg2.h client.cpp client.h server.cpp server.h common/hefile.cpp common/hefile.h -Llib  -o test $(TFHE_LIB) -ltfhe-math

server: libtfhe-math.a
	$(CC) $(CFLAGS) server_main.cpp reg2.cpp reg2.h client.cpp client.h server.cpp server.h common/hefile.cpp common/hefile.h -Llib  -o server $(TFHE_LIB) -ltfhe-math

client: libtfhe-math.a
	$(CC) $(CFLAGS) client_main.cpp reg2.cpp reg2.h client.cpp client.h server.cpp server.h common/hefile.cpp common/hefile.h -Llib -o client $(TFHE_LIB) -ltfhe-math

clean:
	rm -rf test server client *.o lib
