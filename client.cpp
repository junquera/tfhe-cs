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
// void verif(){

  // //read the 16 ciphertexts of the result
  // LweSample* answer = new_gate_bootstrapping_ciphertext_array(nb_bits, params3);
	//
  // //import the 32 ciphertexts from the answer file
  // answer_data = fopen("answer.data","rb");
  // for (int i=0; i<nb_bits; i++)
  //     import_gate_bootstrapping_ciphertext_fromFile(answer_data, &answer[i], params3);
  // fclose(answer_data);
	//
  // //decrypt and rebuild the 16-bit plaintext answer
  // int32_t int_answer = 0;
  // for (int i=0; i<nb_bits; i++) {
  //     int ai = bootsSymDecrypt(&answer[i], key);
  //     int_answer |= (ai<<i);
  // }
	//
	// float float_answer = hint2float(int_answer, float_bits);
	//
  // printf("And the result is: %f\n",float_answer);
  // // printf("And the result is: %d\n",int_answer);
  // printf("I hope you remember what was the question!\n");
	//
  // //clean up all pointers
  // delete_gate_bootstrapping_ciphertext_array(nb_bits, answer);
  // delete_gate_bootstrapping_secret_keyset(key);
// }


// Valores [ (x1, y1), ... , (xn, yn) ]
float cabogata_1417[48][2] = {
  {1.0, 12.2}, {2.0, 10.8}, {3.0, 14.5}, {4.0, 15.6}, {5.0, 20.4}, {6.0, 24.1}, {7.0, 25.8}, {8.0, 26.7}, {9.0, 23.7}, {10.0, 19.3}, {11.0, 16.4}, {12.0, 13.5},
  {1.0, 13.3}, {2.0, 12.5}, {3.0, 14.8}, {4.0, 16.5}, {5.0, 18.3}, {6.0, 22.2}, {7.0, 26.6}, {8.0, 28.0}, {9.0, 25.3}, {10.0, 22.0}, {11.0, 15.7}, {12.0, 14.0},
  {1.0, 14.0}, {2.0, 13.8}, {3.0, 15.7}, {4.0, 18.6}, {5.0, 20.5}, {6.0, 23.5}, {7.0, 25.8}, {8.0, 26.9}, {9.0, 25.1}, {10.0, 22.5}, {11.0, 17.7}, {12.0, 14.0},
  {1.0, 13.1}, {2.0, 13.5}, {3.0, 14.5}, {4.0, 17.9}, {5.0, 21.4}, {6.0, 24.7}, {7.0, 28.7}, {8.0, 28.3}, {9.0, 23.7}, {10.0, 21.1}, {11.0, 17.3}, {12.0, 15.2}
};

float finisterre_1417[48][2] = {
	{1.0, 11.4}, {2.0, 10.9}, {3.0,12.4}, {4.0, 14.9}, {5.0, 15.1}, {6.0, 18.3}, {7.0, 19.3}, {8.0, 19.9}, {9.0, 20.8}, {10.0, 18.8}, {11.0, 13.5}, {12.0, 11.3},
	{1.0, 10.7}, {2.0, 10.0}, {3.0, 11.7}, {4.0, 14.8}, {5.0, 15.9}, {6.0, 17.9}, {7.0, 20.0}, {8.0, 19.2}, {9.0, 17.6}, {10.0, 16.4}, {11.0, 15.5}, {12.0, 14.1},
	{1.0, 12.1}, {2.0, 11.0}, {3.0, 11.1}, {4.0, 12.1}, {5.0, 15.2}, {6.0, 17.6}, {7.0, 19.8}, {8.0, 20.1}, {9.0, 18.7}, {10.0, 16.7}, {11.0, 13.0}, {12.0, 13.1},
	{1.0, 10.4}, {2.0, 11.8}, {3.0, 12.9}, {4.0, 14.3}, {5.0, 17.4}, {6.0, 18.7}, {7.0, 19.9}, {8.0, 19.8}, {9.0, 18.3}, {10.0, 17.8}, {11.0, 13.0}, {12.0, 11.2}
};


void regresion(float** values){

	// Número de bits con los que queremos trabajar
	const int nb_bits = 32;
	const int float_bits = 7;

	HClient client(nb_bits, float_bits);

	TFheGateBootstrappingCloudKeySet* bk;
	client.getCloudKey(bk);

	TFheGateBootstrappingParameterSet* params;
	//if necessary, the params are inside the key
	params = (TFheGateBootstrappingParameterSet*) &bk->params;

	vector<LweSample*> xs, ys;

	int32_t plain;
	LweSample* cipher;
	for(int i = 0; i < 48; i++) {

		// Añade xs
		plain = float2hint(values[i][0], float_bits);
		cipher = new_gate_bootstrapping_ciphertext_array(nb_bits, params);

		client.cifra(cipher, plain);

		xs.push_back(cipher);

		// Añade ys
		plain = float2hint(cabogata_1417[i][1], float_bits);
		cipher = new_gate_bootstrapping_ciphertext_array(nb_bits, params);

		client.cifra(cipher, plain);

		ys.push_back(cipher);
	}

	LweSample* a = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
	LweSample* b = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
	LweSample* c = new_gate_bootstrapping_ciphertext_array(nb_bits, params);

	regresion_cuadratica(a, b, c, xs, ys, float_bits, nb_bits, bk);

	// Descifrar resultado
	int32_t a_int_answer = client.descifra(a);
	int32_t b_int_answer = client.descifra(b);
	int32_t c_int_answer = client.descifra(c);

	float a_float_answer = hint2float(a_int_answer, float_bits);
	float b_float_answer = hint2float(b_int_answer, float_bits);
	float c_float_answer = hint2float(c_int_answer, float_bits);

	cout << "a: " << a_float_answer << "\nb: " << b_float_answer << "\nc: " << c_float_answer << endl;
};
