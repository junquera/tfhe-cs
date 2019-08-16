TFHE_PREFIX=/usr/local #the prefix where you installed tfhe
C_INCLUDE_PATH=$C_INCLUDE_PATH:$TFHE_PREFIX/include
CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$TFHE_PREFIX/include
LIBRARY_PATH=$LIBRARY_PATH:$TFHE_PREFIX/lib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TFHE_PREFIX/lib


all: test server client
	echo "Ok!"

test:
	g++ test.cpp common/arithmetic.cpp reg2.cpp common/arithmetic.h reg2.h client.cpp client.h server.cpp server.h common/hefile.cpp common/hefile.h  -o test -ltfhe-fftw

server:
	g++ server_main.cpp common/arithmetic.cpp reg2.cpp common/arithmetic.h reg2.h client.cpp client.h server.cpp server.h common/hefile.cpp common/hefile.h  -o server -ltfhe-fftw

client:
	g++ client_main.cpp common/arithmetic.cpp reg2.cpp common/arithmetic.h reg2.h client.cpp client.h server.cpp server.h common/hefile.cpp common/hefile.h  -o client -ltfhe-fftw

clean:
	rm test server client
