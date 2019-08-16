#ifndef _REG2_H_INCLUDED_
#define _REG2_H_INCLUDED_

#include <iostream>
#include <vector>

#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>

#include "common/arithmetic.h"
using namespace std;

class RegresionCuadratica {
  public:
    RegresionCuadratica(int _nb_bits, int _float_bits, TFheGateBootstrappingCloudKeySet* _bk);
    void calcula(LweSample* a, LweSample* b, LweSample* c, const vector<LweSample*> xs, const vector<LweSample*> ys, string results_path);
  private:
    int nb_bits;
    int float_bits;

    TFheGateBootstrappingCloudKeySet* bk;

    void initVectores(const vector<LweSample*> xs, const vector<LweSample*> ys, string results_path);
    void calcCuadrados(string results_path);
    void calcDuplas(string results_path);
    void calcComplejos(string results_path);
    void calcC(LweSample* c, string results_path);
    void calcB(LweSample* b, LweSample* c, string results_path);
    void calcA(LweSample* a, LweSample* b, LweSample* c, string results_path);


    LweSample* aux1;
    LweSample* aux2;
    LweSample* aux3;
    LweSample* aux4;

    LweSample* n;
    LweSample* uno;

    LweSample* i;
    LweSample* j;
    LweSample* k;
    LweSample* l;

    LweSample* u;
    LweSample* v;
    LweSample* w;

    // Cuadrados de las variables
    LweSample* i2;
    LweSample* j2;
    LweSample* k2;
    LweSample* l2;

    // Duplas
    LweSample* il;
    LweSample* jk;
    LweSample* jl;
    LweSample* kw;
    LweSample* kv;
    LweSample* ln;
    LweSample* ul;
    LweSample* vl;
    LweSample* wj;

    LweSample* ijkl;
    LweSample* i2l2;
    LweSample* iklw;
    LweSample* il2v;
    LweSample* j2k2;
    LweSample* jk2w;
    LweSample* jklv;
    LweSample* j3l;
    LweSample* jl2n;
    LweSample* k2ln;
};

#endif
