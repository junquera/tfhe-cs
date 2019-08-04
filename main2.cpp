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

int main(){
	const int minimum_lambda = 110;
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



    //clean up all pointers
    delete_gate_bootstrapping_secret_keyset(key);
    delete_gate_bootstrapping_parameters(params);



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
 LweSample* ciphertext1 = new_gate_bootstrapping_ciphertext_array(16, params2);
 LweSample* ciphertext2 = new_gate_bootstrapping_ciphertext_array(16, params2);

 //reads the 2x16 ciphertexts from the cloud file
 FILE* cloud_data = fopen("cloud.data","rb");
 for (int i=0; i<16; i++) import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext1[i], params2);
 for (int i=0; i<16; i++) import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext2[i], params2);
 fclose(cloud_data);

 //do some operations on the ciphertexts: here, we will compute the
 //minimum of the two
 LweSample* result = new_gate_bootstrapping_ciphertext_array(16, params2);
 minimum(result, ciphertext1, ciphertext2, 16, bk);

 //export the 32 ciphertexts to a file (for the cloud)
 FILE* answer_data = fopen("answer.data","wb");
 for (int i=0; i<16; i++) export_gate_bootstrapping_ciphertext_toFile(answer_data, &result[i], params2);
 fclose(answer_data);

 //clean up all pointers
 delete_gate_bootstrapping_ciphertext_array(16, result);
 delete_gate_bootstrapping_ciphertext_array(16, ciphertext2);
 delete_gate_bootstrapping_ciphertext_array(16, ciphertext1);
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
    LweSample* answer = new_gate_bootstrapping_ciphertext_array(16, params3);

    //import the 32 ciphertexts from the answer file
    answer_data = fopen("answer.data","rb");
    for (int i=0; i<16; i++)
        import_gate_bootstrapping_ciphertext_fromFile(answer_data, &answer[i], params3);
    fclose(answer_data);

    //decrypt and rebuild the 16-bit plaintext answer
    int16_t int_answer = 0;
    for (int i=0; i<16; i++) {
        int ai = bootsSymDecrypt(&answer[i], key);
        int_answer |= (ai<<i);
    }

    printf("And the result is: %d\n",int_answer);
    printf("I hope you remember what was the question!\n");

    //clean up all pointers
    delete_gate_bootstrapping_ciphertext_array(16, answer);
    delete_gate_bootstrapping_secret_keyset(key);

	return 0;
}
