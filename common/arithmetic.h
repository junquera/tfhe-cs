#ifndef _ARITMETIC_H_INCLUDED_
#define _ARITMETIC_H_INCLUDED_

#include <iostream>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>


// TODO Optimizaciones
void compare_bit(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* lsb_carry, LweSample* tmp, const TFheGateBootstrappingCloudKeySet* bk);

void equal(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void mayor_igual(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void is_negative(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void negativo(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void minimum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void maximum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void shiftl(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void shiftr(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void u_shiftl(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void u_shiftr(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void add_bit(LweSample* result, LweSample* carry_out, const LweSample* a, const LweSample* b, const LweSample* carry_in, const TFheGateBootstrappingCloudKeySet* bk);

void sum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void resta(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
// void old_multiply(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void multiply(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void divide(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void multiply_float(LweSample* result, const LweSample* a, const LweSample* b, const int float_bits, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void divide_float(LweSample* result, const LweSample* a, const LweSample* b, const int float_bits, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void h_pow(LweSample* result, const LweSample* a, const int n, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void porDiez(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void entreDiez(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void reescala(LweSample* result, const LweSample* a, const int nb_bits_result, const int nb_bits,  const TFheGateBootstrappingCloudKeySet* bk);

int64_t float2hint(float i, int float_bits);
float hint2float(int64_t i, int float_bits);
#endif
