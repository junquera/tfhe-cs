#ifndef _AUX_H_INCLUDED_
#define _AUX_H_INCLUDED_

#include <iostream>
#include <vector>

#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>

#include "functions.h"
using namespace std;
/*
	De https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
*/
inline bool exists_file (string name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

bool retrieveResult(string name, LweSample* result, int nb_bits, const TFheGateBootstrappingParameterSet* params){

  string file_name = name + ".data";

  if(exists_file(file_name)){

    FILE* answer_data = fopen(file_name.c_str(), "rb");
    for (int i=0; i<nb_bits; i++){
      import_gate_bootstrapping_ciphertext_fromFile(answer_data, &result[i], params);
    }

    fclose(answer_data);

    return true;
  } else {
    return false;
  }
}

void saveResult(string name, LweSample* result, int nb_bits, const TFheGateBootstrappingParameterSet* params){

  string file_name = name + ".data";

  FILE* answer_data = fopen(file_name.c_str(), "wb");
  for (int i=0; i<nb_bits; i++) export_gate_bootstrapping_ciphertext_toFile(answer_data, &result[i], params);
  fclose(answer_data);
}

#endif
