#include "client.h"

HClient::HClient(int nb_bits, int float_bits){
	string keyFileName = "secret.key";

	if(exists_file(keyFileName)){
		loadSecretKeyFromFile(keyFileName, key);
	} else {
		generaClaves(key);
	}
};

void HClient::cifra(LweSample* result, int32_t input){
	for(int i = 0; i < nb_bits; i++) {
		bootsSymEncrypt(&result[i], (input>>i)&1, key);
	}
};

int32_t HClient::descifra(LweSample* input){

  int32_t int_answer = 0;
  for (int i=0; i<nb_bits; i++) {
      int ai = bootsSymDecrypt(&input[i], key);
      int_answer |= (ai<<i);
  }

	return int_answer;
};



void HClient::exportCloudKeyToFile(string name){
		//export the cloud key to a file (for the cloud)
	  FILE* cloud_key = fopen(name.c_str(),"wb");
	  export_tfheGateBootstrappingCloudKeySet_toFile(cloud_key, &key->cloud);
	  fclose(cloud_key);
};

void HClient::exportSecretKeyToFile(string name){
	//export the secret key to file for later use
	FILE* secret_key = fopen(name.c_str(),"wb");
	export_tfheGateBootstrappingSecretKeySet_toFile(secret_key, key);
	fclose(secret_key);
};

void HClient::getCloudKey(TFheGateBootstrappingCloudKeySet* &cloudKey){
	cloudKey = (TFheGateBootstrappingCloudKeySet*) &key->cloud;
};
