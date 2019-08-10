#ifndef _AUX_H_INCLUDED_
#define _AUX_H_INCLUDED_

#include <iostream>
#include <vector>

#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>

#include "functions.h"

using namespace std;

bool exists_file (string name);

bool retrieveResult (string name, LweSample* result, int nb_bits, const TFheGateBootstrappingParameterSet* params);

void saveResult(string name, LweSample* result, int nb_bits, const TFheGateBootstrappingParameterSet* params);

#endif
