# configure environment variables
# son necesarios para ejecutar!
TFHE_PREFIX=/usr/local #the prefix where you installed tfhe
export C_INCLUDE_PATH=$C_INCLUDE_PATH:$TFHE_PREFIX/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$TFHE_PREFIX/include
export LIBRARY_PATH=$LIBRARY_PATH:$TFHE_PREFIX/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TFHE_PREFIX/lib

g++ main.cpp functions.cpp reg2.cpp functions.h reg2.h client.cpp client.h server.cpp server.h aux.cpp aux.h  -o main -ltfhe-fftw
g++ client_main.cpp functions.cpp reg2.cpp functions.h reg2.h client.cpp client.h server.cpp server.h aux.cpp aux.h  -o client -ltfhe-fftw
g++ server_main.cpp functions.cpp reg2.cpp functions.h reg2.h client.cpp client.h server.cpp server.h aux.cpp aux.h  -o server -ltfhe-fftw
echo "¡Hecho!"
