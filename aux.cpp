#include "aux.h"

/*
	De https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
*/
bool exists_file (string name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

bool retrieveResult (string name, LweSample* result, int nb_bits, const TFheGateBootstrappingParameterSet* params) {

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

void saveResult(string name, LweSample* result, int nb_bits, const TFheGateBootstrappingParameterSet* params) {

  string file_name = name + ".data";

  FILE* answer_data = fopen(file_name.c_str(), "wb");
  for (int i=0; i<nb_bits; i++) export_gate_bootstrapping_ciphertext_toFile(answer_data, &result[i], params);
  fclose(answer_data);
}


void generaClaves(TFheGateBootstrappingSecretKeySet* &key) {
	// TODO ¿QUé es esto?
	const int minimum_lambda = 110;

	TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(minimum_lambda);
	//generate a random key
	uint32_t seed[] = { 314, 1592, 657 };
	tfhe_random_generator_setSeed(seed, 3);
	key = new_random_gate_bootstrapping_secret_keyset(params);
}

void loadSecretKeyFromFile(string keyFileName, TFheGateBootstrappingSecretKeySet* &key){
	//reads the cloud key from file
	FILE* secret_key = fopen(keyFileName.c_str(),"rb");
	key = new_tfheGateBootstrappingSecretKeySet_fromFile(secret_key);
	fclose(secret_key);
}

void loadCloudKeyFromFile(string keyFileName, TFheGateBootstrappingCloudKeySet* &key){
  FILE* cloud_key = fopen("cloud.key","rb");
  key = new_tfheGateBootstrappingCloudKeySet_fromFile(cloud_key);
  fclose(cloud_key);
}
