#ifndef _REG2_H_INCLUDED_
#define _REG2_H_INCLUDED_

#include <iostream>
#include <vector>

#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>

#include "functions.h"
using namespace std;


void regresion_cuadratica(LweSample* a, LweSample* b, LweSample* c, const vector<LweSample*> xs, const vector<LweSample*> ys, const int float_bits, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

#endif
