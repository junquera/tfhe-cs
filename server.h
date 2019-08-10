#ifndef _SERVER_H_INCLUDED_
#define _SERVER_H_INCLUDED_

#include "aux.h"
class RegresionCuadratica {
  public:
    RegresionCuadratica(int nb_bits, int float_bits, TFheGateBootstrappingCloudKeySet* bk, string results_path);
    void calcula(LweSample* a, LweSample* b, LweSample* c, const vector<LweSample*> xs, const vector<LweSample*> ys);
  private:
    int nb_bits;
    int float_bits;

    TFheGateBootstrappingCloudKeySet* bk;

    string results_path;

    void initVectores(const vector<LweSample*> xs, const vector<LweSample*> ys);
    void calcCuadrados();
    void calcDuplas();
    void calcComplejos();
    void calcC(LweSample* c);
    void calcB(LweSample* b, LweSample* c);
    void calcA(LweSample* a, LweSample* b, LweSample* c);


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

class HServer {
  public:
    HServer(int nb_bits, int float_bits);
    void regresionCuadratica(LweSample* a, LweSample* b, LweSample* c, const vector<LweSample*> xs, const vector<LweSample*> ys, string cloud_key_path, string results_path);
  private:
    int nb_bits;
    int float_bits;
};

#endif