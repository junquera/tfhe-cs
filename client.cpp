#include "aux.h"

class HClient {
	public:
		HClient(){
		}
}

void verif(){

  //read the 16 ciphertexts of the result
  LweSample* answer = new_gate_bootstrapping_ciphertext_array(nb_bits, params3);

  //import the 32 ciphertexts from the answer file
  answer_data = fopen("answer.data","rb");
  for (int i=0; i<nb_bits; i++)
      import_gate_bootstrapping_ciphertext_fromFile(answer_data, &answer[i], params3);
  fclose(answer_data);

  //decrypt and rebuild the 16-bit plaintext answer
  int32_t int_answer = 0;
  for (int i=0; i<nb_bits; i++) {
      int ai = bootsSymDecrypt(&answer[i], key);
      int_answer |= (ai<<i);
  }

	float float_answer = hint2float(int_answer, float_bits);

  printf("And the result is: %f\n",float_answer);
  // printf("And the result is: %d\n",int_answer);
  printf("I hope you remember what was the question!\n");

  //clean up all pointers
  delete_gate_bootstrapping_ciphertext_array(nb_bits, answer);
  delete_gate_bootstrapping_secret_keyset(key);
}

int32_t descifra(LweSample* answer, int nb_bits, TFheGateBootstrappingSecretKeySet* key){

  int32_t int_answer = 0;
  for (int i=0; i<nb_bits; i++) {
      int ai = bootsSymDecrypt(&answer[i], key);
      int_answer |= (ai<<i);
  }

	return int_answer;
}

void generaClaves(TFheGateBootstrappingSecretKeySet* &key, TFheGateBootstrappingParameterSet* &params){

	if(exists_file("secret.key")){
		//reads the cloud key from file
		FILE* secret_key = fopen("secret.key","rb");
		key = new_tfheGateBootstrappingSecretKeySet_fromFile(secret_key);
		fclose(secret_key);

		//if necessary, the params are inside the key
		params = key->params;
	} else {
		// TODO ¿QUé es esto?
		const int minimum_lambda = 110;

		params = new_default_gate_bootstrapping_parameters(minimum_lambda);
		//generate a random key
		uint32_t seed[] = { 314, 1592, 657 };
		tfhe_random_generator_setSeed(seed, 3);
		key = new_random_gate_bootstrapping_secret_keyset(params);

		//export the secret key to file for later use
		FILE* secret_key = fopen("secret.key","wb");
		export_tfheGateBootstrappingSecretKeySet_toFile(secret_key, key);
		fclose(secret_key);

		//export the cloud key to a file (for the cloud)
    FILE* cloud_key = fopen("cloud.key","wb");
    export_tfheGateBootstrappingCloudKeySet_toFile(cloud_key, &key->cloud);
    fclose(cloud_key);
	}

}


// Valores [ (x1, y1), ... , (xn, yn) ]
float cabogata_1417[48][2] = {
  {1.0, 12.2}, {2.0, 10.8}, {3.0, 14.5}, {4.0, 15.6}, {5.0, 20.4}, {6.0, 24.1}, {7.0, 25.8}, {8.0, 26.7}, {9.0, 23.7}, {10.0, 19.3}, {11.0, 16.4}, {12.0, 13.5},
  {1.0, 13.3}, {2.0, 12.5}, {3.0, 14.8}, {4.0, 16.5}, {5.0, 18.3}, {6.0, 22.2}, {7.0, 26.6}, {8.0, 28.0}, {9.0, 25.3}, {10.0, 22.0}, {11.0, 15.7}, {12.0, 14.0},
  {1.0, 14.0}, {2.0, 13.8}, {3.0, 15.7}, {4.0, 18.6}, {5.0, 20.5}, {6.0, 23.5}, {7.0, 25.8}, {8.0, 26.9}, {9.0, 25.1}, {10.0, 22.5}, {11.0, 17.7}, {12.0, 14.0},
  {1.0, 13.1}, {2.0, 13.5}, {3.0, 14.5}, {4.0, 17.9}, {5.0, 21.4}, {6.0, 24.7}, {7.0, 28.7}, {8.0, 28.3}, {9.0, 23.7}, {10.0, 21.1}, {11.0, 17.3}, {12.0, 15.2}
};

void regresion(float[][] values){

	// Generación de claves y parametros
	TFheGateBootstrappingSecretKeySet* key;
	TFheGateBootstrappingParameterSet* params;

	generaClaves(key, params);


  // Número de bits con los que queremos trabajar
  const int nb_bits = 32;
	const int float_bits = 7;


	vector<LweSample*> xs, ys;

	int32_t plain;
	LweSample* cipher;
	for(int i = 0; i < 48; i++) {

		// Añade xs
		plain = float2hint(values[i][0], float_bits);
		cipher = new_gate_bootstrapping_ciphertext_array(nb_bits, params);

		for (int j=0; j<nb_bits; j++)
			bootsSymEncrypt(&cipher[j], (plain>>j)&1, key);

		xs.push_back(cipher);

		// Añade ys
		plain = float2hint(cabogata_1417[i][1], float_bits);
		cipher = new_gate_bootstrapping_ciphertext_array(nb_bits, params);

		for (int j=0; j<nb_bits; j++)
			bootsSymEncrypt(&cipher[j], (plain>>j)&1, key);

		ys.push_back(cipher);
	}

	LweSample* a = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
	LweSample* b = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
	LweSample* c = new_gate_bootstrapping_ciphertext_array(nb_bits, params);

	const TFheGateBootstrappingCloudKeySet* bk = &key->cloud;

	regresion_cuadratica(a, b, c, xs, ys, float_bits, nb_bits, bk);

	// Descifrar resultado
	int32_t a_int_answer = 0;
	int32_t b_int_answer = 0;
	int32_t c_int_answer = 0;

	for (int i=0; i<nb_bits; i++) {
			int ai = bootsSymDecrypt(&a[i], key);
			a_int_answer |= (ai<<i);
			int bi = bootsSymDecrypt(&b[i], key);
			b_int_answer |= (bi<<i);
			int ci = bootsSymDecrypt(&c[i], key);
			c_int_answer |= (ci<<i);
	}

	float a_float_answer = hint2float(a_int_answer, float_bits);
	float b_float_answer = hint2float(b_int_answer, float_bits);
	float c_float_answer = hint2float(c_int_answer, float_bits);

	cout << "a: " << a_float_answer << "\nb: " << b_float_answer << "\nc: " << c_float_answer << endl;
}
