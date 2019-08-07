#include <iostream>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include "functions.h"

using namespace std;

int main(){
  // TODO ¿QUé es esto?
	const int minimum_lambda = 110;

  // Número de bits con los que queremos trabajar
  // IMPORTANTE PARA LA MULTIPLICACIÓN
  const int nb_bits = 16;

	TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(minimum_lambda);
   //generate a random key
    uint32_t seed[] = { 314, 1592, 657 };
    tfhe_random_generator_setSeed(seed,3);
    TFheGateBootstrappingSecretKeySet* key = new_random_gate_bootstrapping_secret_keyset(params);

    //export the secret key to file for later use
    FILE* secret_key = fopen("secret.key","wb");
    export_tfheGateBootstrappingSecretKeySet_toFile(secret_key, key);
    fclose(secret_key);

    //export the cloud key to a file (for the cloud)
    FILE* cloud_key = fopen("cloud.key","wb");
    export_tfheGateBootstrappingCloudKeySet_toFile(cloud_key, &key->cloud);
    fclose(cloud_key);


    //generate encrypt the 16 bits of 2017
   int16_t plaintext1 = 16384;
   LweSample* ciphertext1 = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
   for (int i=0; i<nb_bits; i++) {
       bootsSymEncrypt(&ciphertext1[i], (plaintext1>>i)&1, key);
   }

   //generate encrypt the 16 bits of 42
   int16_t plaintext2 = 8192;
   LweSample* ciphertext2 = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
   for (int i=0; i<nb_bits; i++) {
       bootsSymEncrypt(&ciphertext2[i], (plaintext2>>i)&1, key);
   }

   printf("Hi there! Today, I will ask the cloud what is the minimum between %d and %d\n",plaintext1, plaintext2);

   //export the 2x16 ciphertexts to a file (for the cloud)
   FILE* cloud_data = fopen("cloud.data","wb");
   for (int i=0; i<nb_bits; i++)
       export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext1[i], params);
   for (int i=0; i<nb_bits; i++)
       export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext2[i], params);
   fclose(cloud_data);

   //clean up all pointers
   delete_gate_bootstrapping_ciphertext_array(nb_bits, ciphertext2);
   delete_gate_bootstrapping_ciphertext_array(nb_bits, ciphertext1);
		/**
		SERVER!
		*/

		//reads the cloud key from file
   cloud_key = fopen("cloud.key","rb");
   TFheGateBootstrappingCloudKeySet* bk = new_tfheGateBootstrappingCloudKeySet_fromFile(cloud_key);
   fclose(cloud_key);

   //if necessary, the params are inside the key
   const TFheGateBootstrappingParameterSet* params2 = bk->params;

   //read the 2x16 ciphertexts
   ciphertext1 = new_gate_bootstrapping_ciphertext_array(nb_bits, params2);
   ciphertext2 = new_gate_bootstrapping_ciphertext_array(nb_bits, params2);

   //reads the 2x16 ciphertexts from the cloud file
   cloud_data = fopen("cloud.data","rb");
   for (int i=0; i<nb_bits; i++) import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext1[i], params2);
   for (int i=0; i<nb_bits; i++) import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext2[i], params2);
   fclose(cloud_data);

   //do some operations on the ciphertexts: here, we will compute the
   //minimum of the two
   LweSample* result = new_gate_bootstrapping_ciphertext_array(nb_bits, params2);

   for(int i=0; i < nb_bits; i++)
    bootsCONSTANT(&result[i], 0, bk);

   // minimum(result, ciphertext1, ciphertext2, 16, bk);
   // maximum  (result, ciphertext1, ciphertext2, nb_bits, bk);
   // sum(result, ciphertext1, ciphertext2, 16, bk);
   // resta(result, ciphertext1, ciphertext2, 16, bk);
   // multiply(result, ciphertext1, ciphertext2, nb_bits, bk);
   divide(result, ciphertext1, ciphertext2, nb_bits, bk);
   // mayor_igual(result, ciphertext2, ciphertext1, nb_bits, bk);
  // is_negative(result, ciphertext1, nb_bits, bk);
   //export the 32 ciphertexts to a file (for the cloud)
   FILE* answer_data = fopen("answer.data","wb");
   for (int i=0; i<nb_bits; i++) export_gate_bootstrapping_ciphertext_toFile(answer_data, &result[i], params2);
   fclose(answer_data);

   //clean up all pointers
   delete_gate_bootstrapping_ciphertext_array(nb_bits, result);
   delete_gate_bootstrapping_ciphertext_array(nb_bits, ciphertext2);
   delete_gate_bootstrapping_ciphertext_array(nb_bits, ciphertext1);
   delete_gate_bootstrapping_cloud_keyset(bk);

  /**
  VERIF!
  */


    //reads the cloud key from file
    secret_key = fopen("secret.key","rb");
    key = new_tfheGateBootstrappingSecretKeySet_fromFile(secret_key);
    fclose(secret_key);

    //if necessary, the params are inside the key
    const TFheGateBootstrappingParameterSet* params3 = key->params;

    //read the 16 ciphertexts of the result
    LweSample* answer = new_gate_bootstrapping_ciphertext_array(nb_bits, params3);

    //import the 32 ciphertexts from the answer file
    answer_data = fopen("answer.data","rb");
    for (int i=0; i<nb_bits; i++)
        import_gate_bootstrapping_ciphertext_fromFile(answer_data, &answer[i], params3);
    fclose(answer_data);

    //decrypt and rebuild the 16-bit plaintext answer
    int16_t int_answer = 0;
    for (int i=0; i<nb_bits; i++) {
        int ai = bootsSymDecrypt(&answer[i], key);
        int_answer |= (ai<<i);
    }

    printf("And the result is: %d\n",int_answer);
    printf("I hope you remember what was the question!\n");

    //clean up all pointers
    delete_gate_bootstrapping_ciphertext_array(nb_bits, answer);
    delete_gate_bootstrapping_secret_keyset(key);

	return 0;
}
