
#include "functions.h"
using namespace std;

void is_negative(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  bootsCOPY(&result[0], &a[nb_bits-1], bk);
}

// elementary full comparator gate that is used to compare the i-th bit:
//   input: ai and bi the i-th bit of a and b
//          lsb_carry: the result of the comparison on the lowest bits
//   algo: if (a==b) return lsb_carry else return b
void compare_bit(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* lsb_carry, LweSample* tmp, const TFheGateBootstrappingCloudKeySet* bk) {
    bootsXNOR(tmp, a, b, bk);
    bootsMUX(result, tmp, lsb_carry, a, bk);
}

// Returns a == b
void equal(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){

  LweSample* tmps = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  bootsCONSTANT(&result[0], 1, bk);

  for(int i = 0; i < nb_bits; i++){
    bootsXNOR(&tmps[0], &a[i], &b[i], bk);
    bootsAND(&result[0], &result[0], &tmps[0], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(2, tmps);

}


// this function compares two multibit words, and puts the min in result
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
// this function compares two multibit words, and puts the min in result
void maximum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
  LweSample* min = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* eq = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  minimum(min, a, b, nb_bits, bk);

  // if a == min, max = b; else max = a;
  equal(eq, a, min, nb_bits, bk);
  //t
  for (int i=0; i<nb_bits; i++) {
      bootsMUX(&result[i], &eq[0], &b[i], &a[i], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(nb_bits, min);
  delete_gate_bootstrapping_ciphertext_array(2, eq);
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

// Devuelve -a
void negativo(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
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
    bootsNOT(&restando[i], &a[i], bk);
  sum(aux, restando, uno, nb_bits, bk);

  for(int i=0; i < nb_bits; i++){
    bootsCOPY(&result[i], &aux[i], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(nb_bits, restando);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, uno);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);
}

void resta(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* restando = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  negativo(restando, b, nb_bits, bk);
  sum(result, a, restando, nb_bits, bk);

  delete_gate_bootstrapping_ciphertext_array(nb_bits, restando);
}

// TODO Nueva multiplicación
void old_multiply(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
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

  for(int i = 0; i < pow(2, nb_bits); i++){

    equal(tmps, aux, b, nb_bits, bk);

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

void multiply(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {

  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  // Parámetros para tener en cuenta numeros negativos
  LweSample* negatA = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* negatB = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* opA = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* opB = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* correctorA = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* correctorB = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  LweSample* corrige = new_gate_bootstrapping_ciphertext_array(2, bk->params);


  // TODO Ajustar numero de bits para que: nb(result) = nb(a)+nb(b)
  for(int i = 0; i < nb_bits; i++){
    bootsCONSTANT(&aux[i], 0, bk);
    bootsCONSTANT(&aux2[i], 0, bk);
    bootsCONSTANT(&negatA[i], 0, bk);
    bootsCONSTANT(&negatB[i], 0, bk);
    bootsCONSTANT(&opA[i], 0, bk);
    bootsCONSTANT(&opB[i], 0, bk);
    bootsCONSTANT(&result[i], 0, bk);
  }

  for(int i = 0; i < 2; i++){
    bootsCONSTANT(&correctorA[i], 0, bk);
    bootsCONSTANT(&correctorB[i], 0, bk);
    bootsCONSTANT(&corrige[i], 0, bk);
  }

  negativo(negatA, a, nb_bits, bk);
  negativo(negatB, b, nb_bits, bk);

  /**
    Ponemos los dos números en positivo
    Usamos minimum porque los numeros negativos se codifican con un 1 delante,
    y se considerarían mayores que los negativos
  */
  minimum(opA, negatA, a, nb_bits, bk);
  minimum(opB, negatB, b, nb_bits, bk);

  equal(correctorA, negatA, opA, nb_bits, bk);
  equal(correctorB, negatB, opB, nb_bits, bk);

  /**
    Podría sustituirse todo por:
    bootXOR(&correctorA[0], &a[nb_bits], &a[nb_bits], bk); ?
  */
  // Si solo uno de los dos es negativo, el resultado es negativo
  bootsXOR(corrige, correctorA, correctorB, bk);

  // Multiplica opA * opB
  for(int i = 0; i < (nb_bits/2); i++) {

    // Resetea aux
    for(int j = 0; j < nb_bits; j++){
      bootsCONSTANT(&aux[j], 0, bk);
      bootsCONSTANT(&aux2[j], 0, bk);
    }

    for(int j = 0; j < (nb_bits/2) + 1; j++) {
      bootsAND(&aux[j+i] , &opA[i], &opB[j], bk);
    }

    sum(aux2, aux, result, nb_bits, bk);


    for(int j = 0; j < nb_bits; j++) {
      bootsCOPY(&result[j], &aux2[j], bk);
    }
  }

  // Determinamos si devolver el resultado positivo o negativo
  negativo(aux, result, nb_bits, bk);

  for(int i = 0; i < nb_bits; i++){
    bootsMUX(&result[i], &corrige[0], &aux[i], &result[i], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux2);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, negatA);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, negatB);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, opA);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, opB);
  delete_gate_bootstrapping_ciphertext_array(2, correctorA);
  delete_gate_bootstrapping_ciphertext_array(2, correctorB);
  delete_gate_bootstrapping_ciphertext_array(2, corrige);
}

void mayor(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  maximum(aux, a, b, nb_bits, bk);

  equal(result, aux, a, nb_bits, bk);
}

void mayor_igual(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* may = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* eq = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  mayor(may, a, b, nb_bits, bk);
  equal(eq, a, b, nb_bits, bk);

  bootsOR(&result[0], &may[0], &eq[0], bk);

}


// TODO shiftr, shiftl, divide con negativos
void shiftl(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  for(int i = 0; i < nb_bits; i++){
    bootsCOPY(&result[i], &a[i], bk);
  }
  for(int i = 0; i < posiciones; i++){
    for(int j = 1; j < nb_bits; j++)
      bootsCOPY(&aux[j], &result[j-1], bk);

    bootsCONSTANT(&aux[0], 0, bk);

    for(int j = 0; j < nb_bits; j++)
      bootsCOPY(&result[j], &aux[j], bk);
  }

}

void shiftr(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  for(int i = 0; i < nb_bits; i++){
    bootsCOPY(&result[i], &a[i], bk);
  }

  for(int i = 0; i < posiciones; i++){

    for(int j = 0; j < nb_bits - 1; j++)
      bootsCOPY(&aux[j], &result[j+1], bk);

    bootsCONSTANT(&aux[nb_bits - 1], 0, bk);

    for(int j = 0; j < nb_bits; j++)
      bootsCOPY(&result[j], &aux[j], bk);
  }

}

void divide(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {

  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* gt = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* bit = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  LweSample* dividendo = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* divisor = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* cociente = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* resto = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  for(int i = 0; i < nb_bits; i++){
    bootsCONSTANT(&aux[i], 0, bk);
    bootsCONSTANT(&aux2[i], 0, bk);
    bootsCOPY(&dividendo[i], &a[i], bk);
    bootsCONSTANT(&divisor[i], 0, bk);
    bootsCONSTANT(&cociente[i], 0, bk);
    bootsCONSTANT(&resto[i], 0, bk);
  }

  for(int i = 0; i < 2; i++){
    bootsCONSTANT(&gt[i], 0, bk);
    bootsCONSTANT(&bit[i], 0, bk);
  }

  int padding = nb_bits/2;

  shiftl(divisor, b, padding - 1, nb_bits, bk);

  for(int i = 0; i < padding; i++) {
    // gt = dividendo >= divisor
    mayor_igual(gt, dividendo, divisor, nb_bits, bk);

    bootsCOPY(&cociente[padding-i-1], &gt[0], bk);

    // resto = gt? sub(dividendo, divisor) : resto
    resta(aux, dividendo, divisor, nb_bits, bk);
    for(int j = 0; j < nb_bits; j++){
      bootsMUX(&aux2[j], &gt[0], &aux[j], &dividendo[j], bk);
    }
    for(int j = 0; j < nb_bits; j++){
      bootsCOPY(&resto[j], &aux2[j], bk);
    }

    // dividendo = gt ? resto : dividendo
    for(int j = 0; j < nb_bits; j++){
      bootsMUX(&aux2[j], &gt[0], &resto[j], &dividendo[j], bk);
    }
    for(int j = 0; j < nb_bits; j++){
      bootsCOPY(&dividendo[j], &aux2[j], bk);
    }

    // divisor = shiftr(divisor, 1)
    shiftr(aux, divisor, 1, nb_bits, bk);
    for(int j = 0; j < nb_bits; j++)
      bootsCOPY(&divisor[j], &aux[j], bk);
  }

  for(int i = 0; i < nb_bits; i++)
    bootsCOPY(&result[i], &cociente[i], bk);

  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);
}