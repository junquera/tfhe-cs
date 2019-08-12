#include <iostream>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <ctime>
#include "functions.h"
#include "reg2.h"

using namespace std;

void test(){
  // TODO ¿QUé es esto?
	const int minimum_lambda = 110;

  // Número de bits con los que queremos trabajar
  const int nb_bits = 64;

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

		int float_bits = 7;
		float a1 = 6084;
		float a2 = 4.8;

    //generate encrypt the 16 bits of 2017
   int64_t plaintext1 = float2hint(a1, float_bits);
   LweSample* ciphertext1 = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
   for (int i=0; i<nb_bits; i++) {
       bootsSymEncrypt(&ciphertext1[i], (plaintext1>>i)&1, key);
   }

   //generate encrypt the 16 bits of 42
   int64_t plaintext2 = float2hint(a2, float_bits);
   LweSample* ciphertext2 = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
   for (int i=0; i<nb_bits; i++) {
       bootsSymEncrypt(&ciphertext2[i], (plaintext2>>i)&1, key);
   }

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

  time_t t0 = time(NULL);
   // minimum(result, ciphertext1, ciphertext2, 16, bk);
   // maximum  (result, ciphertext1, ciphertext2, nb_bits, bk);
   // sum(result, ciphertext1, ciphertext2, 16, bk);
   // resta(result, ciphertext1, ciphertext2, 16, bk);
   // multiply(result, ciphertext1, ciphertext2, nb_bits, bk);
	 // divide_float(result, ciphertext1, ciphertext2, float_bits, nb_bits, bk);
   multiply_float(result, ciphertext1, ciphertext1, float_bits, nb_bits, bk);
   // pow(result, ciphertext2, 2, nb_bits, bk);
   // divide(result, ciphertext1, ciphertext2, nb_bits, bk);
   // mayor_igual(result, ciphertext2, ciphertext1, nb_bits, bk);
  // is_negative(result, ciphertext1, nb_bits, bk);

	time_t t = time(NULL);
	cout << "Time (" << nb_bits << " bits): " <<  t - t0 << "s" << endl;


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
    int64_t int_answer = 0;
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

void test_times(){

  // TODO ¿QUé es esto?
	const int minimum_lambda = 110;
	const int float_bits = 10;

  cout << "op,bits,a,b,time(s)" << endl;

  // Entre 4 y 64 bits
  for(int i = 2; i < 7; i++){
    // Número de bits con los que queremos trabajar
    const int nb_bits = pow(2, i);

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
     int64_t plaintext1 = rand() % ((int64_t) pow(2, nb_bits - 2));
     LweSample* ciphertext1 = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
     for (int i=0; i<nb_bits; i++) {
         bootsSymEncrypt(&ciphertext1[i], (plaintext1>>i)&1, key);
     }

     //generate encrypt the 16 bits of 42
     int64_t plaintext2 = rand() % ((int64_t) pow(2, nb_bits - 2));
     LweSample* ciphertext2 = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
     for (int i=0; i<nb_bits; i++) {
         bootsSymEncrypt(&ciphertext2[i], (plaintext2>>i)&1, key);
     }


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

    time_t t0, t;

    t0 = time(NULL);
    equal(result, ciphertext1, ciphertext2, nb_bits, bk);
    t = time(NULL);
    cout << "equal," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    is_negative(result, ciphertext1, nb_bits, bk);
    t = time(NULL);
    cout << "is_negative," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    minimum(result, ciphertext1, ciphertext2, nb_bits, bk);
    t = time(NULL);
    cout << "minimum," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    maximum(result, ciphertext1, ciphertext2, nb_bits, bk);
    t = time(NULL);
    cout << "maximum," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    sum(result, ciphertext1, ciphertext2, nb_bits, bk);
    t = time(NULL);
    cout << "sum," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    negativo(result, ciphertext1, nb_bits, bk);
    t = time(NULL);
    cout << "negativo," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    resta(result, ciphertext1, ciphertext2, nb_bits, bk);
    t = time(NULL);
    cout << "resta," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    multiply(result, ciphertext1, ciphertext2, nb_bits, bk);
    t = time(NULL);
    cout << "multiply," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    multiply_float(result, ciphertext1, ciphertext2, float_bits, nb_bits, bk);
    t = time(NULL);
    cout << "multiply_float," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    mayor(result, ciphertext1, ciphertext2, nb_bits, bk);
    t = time(NULL);
    cout << "mayor," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    mayor_igual(result, ciphertext1, ciphertext2, nb_bits, bk);
    t = time(NULL);
    cout << "mayor_igual," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    shiftl(result, ciphertext1, rand() % (nb_bits - 1), nb_bits, bk);
    t = time(NULL);
    cout << "shiftl," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    shiftr(result, ciphertext1, rand() % (nb_bits - 1), nb_bits, bk);
    t = time(NULL);
    cout << "shiftr," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    divide(result, ciphertext1, ciphertext2, nb_bits, bk);
    t = time(NULL);
    cout << "divide," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    divide_float(result, ciphertext1, ciphertext2, float_bits, nb_bits, bk);
    t = time(NULL);
    cout << "divide_float," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    porDiez(result, ciphertext1, nb_bits, bk);
    t = time(NULL);
    cout << "porDiez," << nb_bits << "," << t - t0 << endl;

    t0 = time(NULL);
    entreDiez(result, ciphertext1, nb_bits, bk);
    t = time(NULL);
    cout << "entreDiez," << nb_bits << "," << t - t0 << endl;

  }
}


void regresion(){
	// TODO ¿QUé es esto?
	const int minimum_lambda = 110;

  // Número de bits con los que queremos trabajar
  const int nb_bits = 64;
	const int float_bits = 10;

	TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(minimum_lambda);
	//generate a random key
	uint32_t seed[] = { 314, 1592, 657 };
	tfhe_random_generator_setSeed(seed, 3);
	TFheGateBootstrappingSecretKeySet* key = new_random_gate_bootstrapping_secret_keyset(params);

	float cabogata_1417[48][2] = {
    {1.0, 12.2}, {2.0, 10.8}, {3.0, 14.5}, {4.0, 15.6}, {5.0, 20.4}, {6.0, 24.1}, {7.0, 25.8}, {8.0, 26.7}, {9.0, 23.7}, {10.0, 19.3}, {11.0, 16.4}, {12.0, 13.5},
    {1.0, 13.3}, {2.0, 12.5}, {3.0, 14.8}, {4.0, 16.5}, {5.0, 18.3}, {6.0, 22.2}, {7.0, 26.6}, {8.0, 28.0}, {9.0, 25.3}, {10.0, 22.0}, {11.0, 15.7}, {12.0, 14.0},
    {1.0, 14.0}, {2.0, 13.8}, {3.0, 15.7}, {4.0, 18.6}, {5.0, 20.5}, {6.0, 23.5}, {7.0, 25.8}, {8.0, 26.9}, {9.0, 25.1}, {10.0, 22.5}, {11.0, 17.7}, {12.0, 14.0},
    {1.0, 13.1}, {2.0, 13.5}, {3.0, 14.5}, {4.0, 17.9}, {5.0, 21.4}, {6.0, 24.7}, {7.0, 28.7}, {8.0, 28.3}, {9.0, 23.7}, {10.0, 21.1}, {11.0, 17.3}, {12.0, 15.2}
	};
	vector<LweSample*> xs;
	vector<LweSample*> ys;

	int64_t plain;
	LweSample* cipher;

	for(int i = 0; i < 48; i++) {
			// Añade xs
			plain = float2hint(cabogata_1417[i][0], float_bits);
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
	int64_t a_int_answer = 0;
	int64_t b_int_answer = 0;
	int64_t c_int_answer = 0;

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

int main(){
	// regresion();
	// test_times();
	test();
	return 0;
}
