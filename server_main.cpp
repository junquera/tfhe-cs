#include "server.h"

#include <string>
#include <iostream>
#include <sstream>

int main(){

  TFheGateBootstrappingCloudKeySet* bk;
	loadCloudKeyFromFile("cloud.key", bk);

	TFheGateBootstrappingParameterSet* params;
	//if necessary, the params are inside the key
	params = (TFheGateBootstrappingParameterSet*) bk->params;


  int nb_bits = 32;
  int float_bits = 7;

  int values = 48;

  vector<LweSample*> xs, ys;

  stringstream stringStream;
  for(int i = 0; i < values; i++){
    LweSample* resultX = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
    stringStream.str("");
    stringStream << "ciudad_AX" << i;
    retrieveResult(stringStream.str(), resultX, nb_bits, params);
    xs.push_back(resultX);

    LweSample* resultY = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
    stringStream.str("");
    stringStream << "ciudad_AY" << i;
    retrieveResult(stringStream.str(), resultY, nb_bits, params);
    ys.push_back(resultY);
  }

  LweSample* a = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
  LweSample* b = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
  LweSample* c = new_gate_bootstrapping_ciphertext_array(nb_bits, params);

  HServer server(nb_bits, float_bits);
  server.regresionCuadratica(a, b, c, xs, ys, "cloud.key", "results");

  return 0;
}
