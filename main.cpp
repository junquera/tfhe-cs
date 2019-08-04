#include <iostream>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>

using namespace std;
// elementary full comparator gate that is used to compare the i-th bit:
//   input: ai and bi the i-th bit of a and b
//          lsb_carry: the result of the comparison on the lowest bits
//   algo: if (a==b) return lsb_carry else return b
void compare_bit(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* lsb_carry, LweSample* tmp, const TFheGateBootstrappingCloudKeySet* bk) {
    bootsXNOR(tmp, a, b, bk);
    bootsMUX(result, tmp, lsb_carry, a, bk);
}

// this function compares two multibit words, and puts the max in result
void minimum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
    LweSample* tmps = new_gate_bootstrapping_ciphertext_array(2, bk->params);

    //initialize the carry to 0
    bootsCONSTANT(&tmps[0], 0, bk);
    //run the elementary comparator gate n times
    for (int i=0; i<nb_bits; i++) {
        compare_bit(&tmps[0], &a[i], &b[i], &tmps[0], &tmps[1], bk);
    }
    //tmps[0] is the result of the comparaison: 0 if a is larger, 1 if b is larger
    //select the max and copy it to the result
    for (int i=0; i<nb_bits; i++) {
        bootsMUX(&result[i], &tmps[0], &b[i], &a[i], bk);
    }

    delete_gate_bootstrapping_ciphertext_array(2, tmps);
}

void add_bit(LweSample* result, LweSample* carry_out, const LweSample* a, const LweSample* b, const LweSample* carry_in, const TFheGateBootstrappingCloudKeySet* bk){

  LweSample* s1 = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* c1 = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* c2 = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  bootsCONSTANT(&s1[0], 0, bk);
  bootsCONSTANT(&c1[0], 0, bk);
  bootsCONSTANT(&c2[0], 0, bk);

  bootsXOR(s1, a, b, bk);
  bootsXOR(result, s1, carry_in, bk);

  bootsAND(c1, s1, carry_in, bk);
  bootsAND(c2, a, b, bk);

  bootsOR(carry_out, c1, c2, bk);

  delete_gate_bootstrapping_ciphertext_array(2, s1);
  delete_gate_bootstrapping_ciphertext_array(2, c1);
  delete_gate_bootstrapping_ciphertext_array(2, c2);

}

void sum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
  LweSample* tmps_carry = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  //initialize the carry to 0
  bootsCONSTANT(&tmps_carry[0], 0, bk);

  //run the elementary comparator gate n times
  for (int i=0; i<nb_bits; i++) {
      add_bit(&result[i], &tmps_carry[0], &a[i], &b[i], &tmps_carry[0], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(2, tmps_carry);

}

void resta(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* restando = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* uno = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  for(int i=0; i < nb_bits; i++){
    bootsCONSTANT(&restando[i], 0, bk);
    bootsCONSTANT(&uno[i], 0, bk);
    bootsCONSTANT(&aux[i], 0, bk);
  }
  bootsCONSTANT(&uno[0], 1, bk);

  // b negativo
  for(int i = 0; i < nb_bits; i++)
    bootsNOT(&restando[i], &b[i], bk);
  sum(aux, restando, uno, nb_bits, bk);

  for(int i=0; i < nb_bits; i++){
    bootsCOPY(&restando[i], &aux[i], bk);
  }

  sum(result, a, restando, nb_bits, bk);
}
void equal(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* tmps = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  bootsCONSTANT(&result[0], 1, bk);

  for(int i = 0; i < nb_bits; i++){
    bootsXNOR(&tmps[0], &a[i], &b[i], bk);
    bootsAND(&result[0], &result[0], &tmps[0], bk);
  }

}

void multiply(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
  LweSample* tmps = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* factor = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* sumando = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* cero = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* uno = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  // TODO result y res_aux tienen que tener el doble de bits que nb_bits
  LweSample* res_aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  bootsCONSTANT(&tmps[0], 0, bk);
  for(int i=0; i < nb_bits; i++){
    bootsCONSTANT(&result[i], 0, bk);
    bootsCONSTANT(&aux[i], 0, bk);
    bootsCONSTANT(&aux2[i], 0, bk);
    bootsCONSTANT(&factor[i], 0, bk);
    bootsCONSTANT(&sumando[i], 0, bk);
    bootsCONSTANT(&cero[i], 0, bk);
    bootsCONSTANT(&uno[i], 0, bk);
  }
  bootsCONSTANT(&uno[0], 1, bk);

  /**
  ALGORITMO DE MULTIPLICACIÓN "CIEGA":
   Repetir 2^nb_bits:

   sumando = (aux == b)? 0 : 1;
   factor = (aux == b)? 0 : a;

   aux += sumando;
   result += factor;
  */

  for(int i = 0; i < pow(2, nb_bits); i++){


    equal(tmps, aux, b, nb_bits, bk); // Creo que esta comparación no es así...

    for(int j = 0; j < nb_bits; j++){
      bootsMUX(&sumando[j], &tmps[0], &cero[j], &uno[j], bk);
      bootsMUX(&factor[j], &tmps[0], &cero[j], &a[j], bk);
    }


    sum(aux2, aux, sumando, nb_bits, bk);
    for(int j = 0; j < nb_bits; j++){
      bootsCOPY(&aux[j], &aux2[j], bk);
    }

    sum(res_aux, result, factor, nb_bits, bk);
    for(int j = 0; j < nb_bits; j++){
      bootsCOPY(&result[j], &res_aux[j], bk);
    }

    cout << "It: " << i+1 << endl;
  }

  delete_gate_bootstrapping_ciphertext_array(2, tmps);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, factor);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, sumando);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, cero);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, uno);
}

int main(){
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
   int16_t plaintext1 = 2;
   LweSample* ciphertext1 = new_gate_bootstrapping_ciphertext_array(nb_bits, params);
   for (int i=0; i<nb_bits; i++) {
       bootsSymEncrypt(&ciphertext1[i], (plaintext1>>i)&1, key);
   }

   //generate encrypt the 16 bits of 42
   int16_t plaintext2 = 3;
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
 // minimum(result, ciphertext1, ciphertext2, 16, bk);
 // sum(result, ciphertext1, ciphertext2, 16, bk);
 resta(result, ciphertext1, ciphertext2, 16, bk);
 // multiply(result, ciphertext1, ciphertext2, nb_bits, bk);


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
