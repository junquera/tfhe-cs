#include <iostream>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <ctime>
#include "tfhe-math/arithmetic.h"
#include "reg2.h"
#include "client.h"

using namespace std;

void test(){

	int nb_bits = 64;
	int float_bits = 10; // Con 7 bits tengo 2 decimales, con 10, 3

  HClient client(nb_bits, float_bits);
  if(exists_file("secret.key"))
    client.setKeysFromFile("secret.key");
  else
    client.genKeys();


	TFheGateBootstrappingCloudKeySet* bk;
	client.getCloudKey(bk);
	//if necessary, the params are inside the key
	const TFheGateBootstrappingParameterSet* params = bk->params;

	float a1 = 92;
	float a2 = 6.41;
	int64_t plaintext1 = float2hint(a1, float_bits);
	int64_t plaintext2 = float2hint(a2, float_bits);

	LweSample* ciphertext1 = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
	LweSample* ciphertext2 = new_gate_bootstrapping_ciphertext_array(nb_bits, params);

	client.cifra(ciphertext1, plaintext1);
	client.cifra(ciphertext2, plaintext2);

	cout << a1 << "|" << a2 << endl;
	cout << plaintext1 << "|" << plaintext2 << endl;

	/**
		SERVER!
	*/

	//do some operations on the ciphertexts: here, we will compute the
	//minimum of the two
	LweSample* result = new_gate_bootstrapping_ciphertext_array(nb_bits, params);

	for(int i=0; i < nb_bits; i++)
		bootsCONSTANT(&result[i], 0, bk);

  time_t t0 = time(NULL);
   // minimum(result, ciphertext1, ciphertext2, nb_bits, bk);
   // maximum  (result, ciphertext1, ciphertext2, nb_bits, bk);
   // add(result, ciphertext1, ciphertext1, nb_bits, bk);
   // sub(result, ciphertext1, ciphertext2, 16, bk);
	 // mult(result, ciphertext1, ciphertext2, nb_bits, bk);
	 // u_shiftr(result, ciphertext1, 1, nb_bits, bk);
	 div_float(result, ciphertext1, ciphertext2, float_bits, nb_bits, bk);
   // mult_float(result, ciphertext1, ciphertext1, float_bits, nb_bits, bk);
   // pow(result, ciphertext2, 2, nb_bits, bk);
   // div(result, ciphertext1, ciphertext2, nb_bits, bk);
   // gte(result, ciphertext1, ciphertext2, nb_bits, bk);
  // is_negative(result, ciphertext1, nb_bits, bk);
	// for(int i=0; i < nb_bits; i++){
	// 	bootsCOPY(&result[i], &ciphertext1[i], bk);
	// }
	cout << "Time (" << nb_bits << " bits): " <<  time(NULL) - t0 << "s" << endl;

  //decrypt and rebuild the 16-bit plaintext answer
  int64_t int_answer = client.descifra(result);
	float float_answer = hint2float(int_answer, float_bits);

  printf("And the result is: %f\n",float_answer);

  delete_gate_bootstrapping_ciphertext_array(nb_bits, result);
}

void test_times(){


  cout << "op,bits,time(s)" << endl;

  // Entre 4 y 64 bits
  for(int i = 2; i < 7; i++){

		// Número de bits con los que queremos trabajar
		const int nb_bits = pow(2, i);
		const int float_bits = (int) nb_bits*0.25;

		HClient client(nb_bits, float_bits);
		client.genKeys();

		TFheGateBootstrappingCloudKeySet* bk;
		client.getCloudKey(bk);
		//if necessary, the params are inside the key
		const TFheGateBootstrappingParameterSet* params = bk->params;

		//generate encrypt the 16 bits of 2017
		int64_t plaintext1 = rand() % ((int64_t) pow(2, nb_bits - 2));
		LweSample* ciphertext1 = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
		client.cifra(ciphertext1, plaintext1);


		//generate encrypt the 16 bits of 42
		int64_t plaintext2 = rand() % ((int64_t) pow(2, nb_bits - 2));
		LweSample* ciphertext2 = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
		client.cifra(ciphertext2, plaintext2);


		/**
		SERVER!
		*/
		LweSample* ciphertext_aux = new_gate_bootstrapping_ciphertext_array(2*nb_bits, params);
		LweSample* result = new_gate_bootstrapping_ciphertext_array(nb_bits, params);

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
		add(result, ciphertext1, ciphertext2, nb_bits, bk);
		t = time(NULL);
		cout << "sum," << nb_bits << "," << t - t0 << endl;

		t0 = time(NULL);
		negativo(result, ciphertext1, nb_bits, bk);
		t = time(NULL);
		cout << "negativo," << nb_bits << "," << t - t0 << endl;

		t0 = time(NULL);
		sub(result, ciphertext1, ciphertext2, nb_bits, bk);
		t = time(NULL);
		cout << "resta," << nb_bits << "," << t - t0 << endl;

		t0 = time(NULL);
		mult(result, ciphertext1, ciphertext2, nb_bits, bk);
		t = time(NULL);
		cout << "multiply," << nb_bits << "," << t - t0 << endl;

		t0 = time(NULL);
		mult_float(result, ciphertext1, ciphertext2, float_bits, nb_bits, bk);
		t = time(NULL);
		cout << "multiply_float," << nb_bits << "," << t - t0 << endl;

		t0 = time(NULL);
		gte(result, ciphertext1, ciphertext2, nb_bits, bk);
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
		u_shiftl(result, ciphertext1, rand() % (nb_bits - 1), nb_bits, bk);
		t = time(NULL);
		cout << "u_shiftl," << nb_bits << "," << t - t0 << endl;

		t0 = time(NULL);
		u_shiftr(result, ciphertext1, rand() % (nb_bits - 1), nb_bits, bk);
		t = time(NULL);
		cout << "u_shiftr," << nb_bits << "," << t - t0 << endl;

		t0 = time(NULL);
		porDiez(result, ciphertext1, nb_bits, bk);
		t = time(NULL);
		cout << "porDiez," << nb_bits << "," << t - t0 << endl;

		t0 = time(NULL);
		entreDiez(result, ciphertext1, nb_bits, bk);
		t = time(NULL);
		cout << "entreDiez," << nb_bits << "," << t - t0 << endl;

		t0 = time(NULL);
		reescala(ciphertext_aux, ciphertext2, 2*nb_bits, nb_bits, bk);
		t = time(NULL);
		cout << "reescala-+," << nb_bits << "," << t - t0 << endl;

		t0 = time(NULL);
		reescala(ciphertext2, ciphertext_aux, nb_bits, 2*nb_bits, bk);
		t = time(NULL);
		cout << "reescala+-," << nb_bits << "," << t - t0 << endl;

		// Tardan más o menos igual...
		t0 = time(NULL);
		div(result, ciphertext1, ciphertext2, nb_bits, bk);
		t = time(NULL);
		cout << "divide," << nb_bits << "," << t - t0 << endl;

		t0 = time(NULL);
		div_float(result, ciphertext1, ciphertext2, float_bits, nb_bits, bk);
		t = time(NULL);
		cout << "divide_float," << nb_bits << "," << t - t0 << endl;

  }
}

int main(){
	test_times();
	// test();
	return 0;
}
