TFHE_PREFIX=/usr/local #the prefix where you installed tfhe
C_INCLUDE_PATH=$C_INCLUDE_PATH:$TFHE_PREFIX/include
CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$TFHE_PREFIX/include
LIBRARY_PATH=$LIBRARY_PATH:$TFHE_PREFIX/lib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TFHE_PREFIX/lib


all: test server client
	echo "Ok!"

arithmetic.o:
	g++ -Wall -c tfhe-math/arithmetic.cpp -o arithmetic.o

lib:
	mkdir -p lib

libtfhe-math.a: lib arithmetic.o
	ar crf lib/libtfhe-math.a arithmetic.o

test: libtfhe-math.a
	g++ -Wall test.cpp reg2.cpp reg2.h client.cpp client.h server.cpp server.h common/hefile.cpp common/hefile.h -Llib  -o test -ltfhe-fftw -ltfhe-math

server: libtfhe-math.a
	g++ -Wall server_main.cpp reg2.cpp reg2.h client.cpp client.h server.cpp server.h common/hefile.cpp common/hefile.h -Llib  -o server -ltfhe-fftw -ltfhe-math

client: libtfhe-math.a
	g++ -Wall client_main.cpp reg2.cpp reg2.h client.cpp client.h server.cpp server.h common/hefile.cpp common/hefile.h -Llib -o client -ltfhe-fftw -ltfhe-math

clean:
	rm -rf test server client *.o lib
