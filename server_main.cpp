#include "server.h"

#include <string>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

int main(){

  TFheGateBootstrappingCloudKeySet* bk;
	loadCloudKeyFromFile("cloud.key", bk);

	TFheGateBootstrappingParameterSet* params;
	//if necessary, the params are inside the key
	params = (TFheGateBootstrappingParameterSet*) bk->params;


  int nb_bits = 64;
  int float_bits = 10;
  int values = 12;


  string data_ub, data_name, res_dest;

  cout << "¿Dónde están los datos? > ";
  cin >> data_ub;
  cout << "¿Cómo se llaman (ciudad_A, ciudad_B...)? > ";
  cin >> data_name;
  cout << "¿Dónde quieres guardar los resultados? > ";
  cin >> res_dest;
  // if(mkdir(res_dest.c_str(), 0644) == 0){
  //   throw "Error creating folder!";
  // }

  vector<LweSample*> xs, ys;
  stringstream stringStream;
  for(int i = 0; i < values; i++){
    LweSample* resultX = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
    stringStream.str("");
    // TODO Cambiar data_ub << "/" por path.join(data_ub, data_name)
    stringStream << data_ub << "/" << data_name << "X" << i;
    retrieveResult(stringStream.str(), resultX, nb_bits, params);
    xs.push_back(resultX);

    LweSample* resultY = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
    stringStream.str("");
    stringStream << data_ub << "/" << data_name << "Y" << i;
    retrieveResult(stringStream.str(), resultY, nb_bits, params);
    ys.push_back(resultY);
  }

  LweSample* a = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
  LweSample* b = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
  LweSample* c = new_gate_bootstrapping_ciphertext_array(nb_bits, params);

  HServer server(nb_bits, float_bits);
  server.regresionCuadratica(a, b, c, xs, ys, "cloud.key", res_dest);

  return 0;
}
