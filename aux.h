#ifndef _AUX_H_INCLUDED_
#define _AUX_H_INCLUDED_

#include <iostream>
#include <vector>

#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>

#include "functions.h"

using namespace std;

// TODO Pensar un buen sistema para poner el path
bool exists_file (string name);
bool retrieveResult (string name, LweSample* result, int nb_bits, const TFheGateBootstrappingParameterSet* params);
void saveResult(string name, LweSample* result, int nb_bits, const TFheGateBootstrappingParameterSet* params);

void loadSecretKeyFromFile(string keyFileName, TFheGateBootstrappingSecretKeySet* &key);
void loadCloudKeyFromFile(string keyFileName, TFheGateBootstrappingCloudKeySet* &key);
void generaClaves(TFheGateBootstrappingSecretKeySet* &key);

#endif
