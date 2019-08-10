#include "aux.h"

/**
  Operaciones de regresión cuadrática:
    - 6       division        1386 segundos/operación
    - 32      multiplicacion  93 segundos/operación
    - 22      suma/resta      6 segundos/operación

  TOTAL: 11424 segundos = 190 minutos = 3.17 horas
*/
class HServer {
  public:
    HServer(int nb_bits, int float_bits);
    void calcula(LweSample* a, LweSample* b, LweSample* c, const vector<LweSample*> xs, const vector<LweSample*> ys, const int float_bits, const int nb_bits);
    void initVectores(const vector<LweSample*> xs, const vector<LweSample*> ys);
    void calcCuadrados();
    void calcDuplas();
    void calcComplejos();
  private:
    TFheGateBootstrappingCloudKeySet* bk;

    int nb_bits;
    int float_bits;

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

HServer::HServer(int nb_bits, int float_bits) {

  FILE* cloud_key = fopen("cloud.key","rb");
  bk = new_tfheGateBootstrappingCloudKeySet_fromFile(cloud_key);

  fclose(cloud_key);

  nb_bits = nb_bits;
  float_bits = float_bits;

  aux1 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  aux2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  aux3 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  aux4 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  n = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  uno = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  i = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  j = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  k = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  l = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  u = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  v = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  w = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  i2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  j2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  k2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  l2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  il = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  jk = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  jl = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  kw = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  kv = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  ln = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  ul = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  vl = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  wj = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  ijkl = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  i2l2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  iklw = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  il2v = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  j2k2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  jk2w = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  jklv = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  j3l = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  jl2n = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  k2ln = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  // Inicializando variables
  for(int ci=0; ci<nb_bits; ci++){
    bootsCONSTANT(&i[ci], 0, bk);
    bootsCONSTANT(&j[ci], 0, bk);
    bootsCONSTANT(&k[ci], 0, bk);
    bootsCONSTANT(&l[ci], 0, bk);
    bootsCONSTANT(&u[ci], 0, bk);
    bootsCONSTANT(&v[ci], 0, bk);
    bootsCONSTANT(&w[ci], 0, bk);
  }

  // Inicializando variables
  // Inicializa n
  for(int ci=0; ci<nb_bits; ci++){
    bootsCONSTANT(&uno[ci], 0, bk);
  }
  bootsCONSTANT(&uno[0], 1, bk);

  // Ajustando a float_bits
  shiftl(aux2, uno, float_bits, nb_bits, bk);
  for(int ci=0; ci < nb_bits; ci++){
    bootsCOPY(&uno[ci], &aux2[ci], bk);
  }
};

void HServer::initVectores(const vector<LweSample*> xs, const vector<LweSample*> ys){

  int values = xs.size();
  if(xs.size() != ys.size()){
    throw "regresion_cuadratica: len(xs) != len(ys)";
  }

  // Inicializa "n"
  for(int ci=0; ci<nb_bits; ci++)
    bootsCONSTANT(&n[ci], (values>>ci)&1, bk);
  shiftl(aux1, n, float_bits, nb_bits, bk);
  for(int ci=0; ci < nb_bits; ci++)
    bootsCOPY(&n[ci], &aux1[ci], bk);


  bool exists_i = retrieveResult("i", i, nb_bits, bk->params);
  bool exists_j = retrieveResult("j", j, nb_bits, bk->params);
  bool exists_k = retrieveResult("k", k, nb_bits, bk->params);
  bool exists_l = retrieveResult("l", l, nb_bits, bk->params);

  bool exists_u = retrieveResult("u", u, nb_bits, bk->params);
  bool exists_v = retrieveResult("v", v, nb_bits, bk->params);
  bool exists_w = retrieveResult("w", w, nb_bits, bk->params);

  if(exists_i & exists_j & exists_k & exists_l & exists_u & exists_v & exists_w) return;

  for(int ci=0 ; ci < values; ci++){
    /*
    for x, y in xys:
    i += x
    j += x**2
    k += x**3
    l += x**4
    */
    LweSample* x = xs[ci];
    LweSample* y = ys[ci];

    LweSample* x2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
    LweSample* x3 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
    LweSample* x4 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

    multiply_float(x2, x, x, float_bits, nb_bits, bk);
    multiply_float(x3, x2, x, float_bits, nb_bits, bk);
    multiply_float(x4, x2, x2, float_bits, nb_bits, bk);

    sum(aux1, i, x, nb_bits, bk);
    sum(aux1, j, x2, nb_bits, bk);
    sum(aux1, k, x3, nb_bits, bk);
    sum(aux1, l, x4, nb_bits, bk);

    for(int cj=0; cj < nb_bits; cj++){
      bootsCOPY(&i[cj], &aux1[cj], bk);
      bootsCOPY(&j[cj], &aux2[cj], bk);
      bootsCOPY(&k[cj], &aux3[cj], bk);
      bootsCOPY(&l[cj], &aux4[cj], bk);
    }

    /*
      for x, y in xys:
        u += y
        v += (x * y)
        w += ((x**2) * y)
    */
    LweSample* xy = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
    LweSample* x2y = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

    multiply_float(xy, x, y, float_bits, nb_bits, bk);
    multiply_float(x2y, x2, y, float_bits, nb_bits, bk);

    sum(aux1, u, y, nb_bits, bk);
    sum(aux2, v, xy, nb_bits, bk);
    sum(aux3, w, x2y, nb_bits, bk);

    for(int cj=0; cj < nb_bits; cj++){
      bootsCOPY(&u[cj], &aux1[cj], bk);
      bootsCOPY(&v[cj], &aux2[cj], bk);
      bootsCOPY(&w[cj], &aux3[cj], bk);
    }
  }
  saveResult("i", i, nb_bits, bk->params);
  saveResult("j", j, nb_bits, bk->params);
  saveResult("k", k, nb_bits, bk->params);
  saveResult("l", l, nb_bits, bk->params);

  saveResult("u", u, nb_bits, bk->params);
  saveResult("v", v, nb_bits, bk->params);
  saveResult("w", w, nb_bits, bk->params);
};

void HServer::calcCuadrados(){

  bool exists_i2 = retrieveResult("i2", i2, nb_bits, bk->params);
  bool exists_j2 = retrieveResult("j2", j2, nb_bits, bk->params);
  bool exists_k2 = retrieveResult("k2", k2, nb_bits, bk->params);
  bool exists_l2 = retrieveResult("l2", l2, nb_bits, bk->params);

  if(exists_i2 & exists_j2 & exists_k2 & exists_l2) return;

  h_pow(i2, k, 2, nb_bits, bk);
  h_pow(j2, j, 2, nb_bits, bk);
  h_pow(k2, k, 2, nb_bits, bk);
  h_pow(l2, k, 2, nb_bits, bk);

  saveResult("i2", i2, nb_bits, bk->params);
  saveResult("j2", j2, nb_bits, bk->params);
  saveResult("k2", k2, nb_bits, bk->params);
  saveResult("l2", l2, nb_bits, bk->params);
};

void HServer::calcDuplas(){

  bool exists_il = retrieveResult("il", il, nb_bits, bk->params);
  bool exists_jk = retrieveResult("jk", jk, nb_bits, bk->params);
  bool exists_jl = retrieveResult("jl", jl, nb_bits, bk->params);
  bool exists_kw = retrieveResult("kw", kw, nb_bits, bk->params);
  bool exists_kv = retrieveResult("kv", kv, nb_bits, bk->params);
  bool exists_ln = retrieveResult("ln", ln, nb_bits, bk->params);
  bool exists_ul = retrieveResult("ul", ul, nb_bits, bk->params);
  bool exists_vl = retrieveResult("vl", vl, nb_bits, bk->params);
  bool exists_wj = retrieveResult("wj", wj, nb_bits, bk->params);

  if(exists_il & exists_jk & exists_jl &
    exists_kw & exists_kv & exists_ln &
    exists_ul & exists_vl & exists_wj) return;

  multiply_float(il, i, l, float_bits, nb_bits, bk);
  multiply_float(jk, j, k, float_bits, nb_bits, bk);
  multiply_float(jl, j, l, float_bits, nb_bits, bk);
  multiply_float(kw, k, w, float_bits, nb_bits, bk);
  multiply_float(kv, k, v, float_bits, nb_bits, bk);
  multiply_float(ln, l, n, float_bits, nb_bits, bk);
  multiply_float(ul, u, l, float_bits, nb_bits, bk);
  multiply_float(vl, v, l, float_bits, nb_bits, bk);
  multiply_float(wj, w, j, float_bits, nb_bits, bk);

  saveResult("il", il, nb_bits, bk->params);
  saveResult("jk", jk, nb_bits, bk->params);
  saveResult("jl", jl, nb_bits, bk->params);
  saveResult("kw", kw, nb_bits, bk->params);
  saveResult("kv", kv, nb_bits, bk->params);
  saveResult("ln", ln, nb_bits, bk->params);
  saveResult("ul", ul, nb_bits, bk->params);
  saveResult("vl", vl, nb_bits, bk->params);
  saveResult("wj", wj, nb_bits, bk->params);
};

void HServer::calcComplejos(){

  bool exists_ijkl = retrieveResult("ijkl", ijkl, nb_bits, bk->params);
  bool exists_i2l2 = retrieveResult("i2l2", i2l2, nb_bits, bk->params);
  bool exists_iklw = retrieveResult("iklw", iklw, nb_bits, bk->params);
  bool exists_il2v = retrieveResult("il2v", il2v, nb_bits, bk->params);
  bool exists_j2k2 = retrieveResult("j2k2", j2k2, nb_bits, bk->params);
  bool exists_jk2w = retrieveResult("jk2w", jk2w, nb_bits, bk->params);
  bool exists_jklv = retrieveResult("jklv", jklv, nb_bits, bk->params);
  bool exists_j3l = retrieveResult("j3l", j3l, nb_bits, bk->params);
  bool exists_jl2n = retrieveResult("jl2n", jl2n, nb_bits, bk->params);
  bool exists_k2ln = retrieveResult("k2ln", k2ln, nb_bits, bk->params);


  if (exists_ijkl & exists_i2l2 & exists_iklw & exists_il2v & exists_j2k2 &
      exists_jk2w & exists_jklv & exists_j3l & exists_jl2n & exists_k2ln)
      return;

  multiply_float(ijkl, il, jk, float_bits, nb_bits, bk);
  multiply_float(i2l2, i2, l2, float_bits, nb_bits, bk);
  multiply_float(iklw, il, kw, float_bits, nb_bits, bk);
  multiply_float(il2v, il, vl, float_bits, nb_bits, bk);
  multiply_float(j2k2, j2, k2, float_bits, nb_bits, bk);
  multiply_float(jk2w, jk, kw, float_bits, nb_bits, bk);
  multiply_float(jklv, jk, vl, float_bits, nb_bits, bk);
  multiply_float(j3l, j2, jl, float_bits, nb_bits, bk);
  multiply_float(jl2n, jl, ln, float_bits, nb_bits, bk);
  multiply_float(k2ln, k2, ln, float_bits, nb_bits, bk);

  saveResult("ijkl", ijkl, nb_bits, bk->params);
  saveResult("i2l2", i2l2, nb_bits, bk->params);
  saveResult("iklw", iklw, nb_bits, bk->params);
  saveResult("il2v", il2v, nb_bits, bk->params);
  saveResult("j2k2", j2k2, nb_bits, bk->params);
  saveResult("jk2w", jk2w, nb_bits, bk->params);
  saveResult("jklv", jklv, nb_bits, bk->params);
  saveResult("j3l", j3l, nb_bits, bk->params);
  saveResult("jl2n", jl2n, nb_bits, bk->params);
  saveResult("k2ln", k2ln, nb_bits, bk->params);

};

void HServer::calcula(LweSample* a, LweSample* b, LweSample* c, const vector<LweSample*> xs, const vector<LweSample*> ys, const int float_bits, const int nb_bits){

  initVectores(xs, ys);
  calcCuadrados();
  calcDuplas();
  calcComplejos();

  /*
    c = (ul - wj)/(ln - j2)
    c += (jklv - jk2w - il2v + iklw)/(jl2n - k2ln - j3l + j2k2)
    r = (j2k2-2(ijkl)+i2l2)/(jl2n - (k2ln - j3l + j2k2))
    c = c / (1 - r)
  */

  // c = (ul - wj)/(ln - j2)
  resta(aux1, ul, wj, nb_bits, bk);
  resta(aux2, ln, j2, nb_bits, bk);
  divide_float(c, aux1, aux2, float_bits, nb_bits, bk);

  // c += (jklv - jk2w - il2v + iklw)/(jl2n - k2ln - j3l + j2k2)
  resta(aux1, jklv, jk2w, nb_bits, bk);
  resta(aux2, aux1, il2v, nb_bits, bk);
  sum(aux1, aux2, iklw, nb_bits, bk);

  resta(aux2, jl2n, k2ln, nb_bits, bk);
  resta(aux3, aux2, j3l, nb_bits, bk);
  sum(aux2, aux3, j2k2, nb_bits, bk);
  divide_float(aux3, aux1, aux2, float_bits, nb_bits, bk);

  sum(aux1, c, aux3, nb_bits, bk);
  for(int ci=0; ci < nb_bits; ci++)
    bootsCOPY(&c[ci], &aux1[ci], bk);


  // r = (j2k2-2(ijkl)+i2l2)/(jl2n - k2ln - j3l + j2k2)
  LweSample* r = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  sum(aux1, ijkl, ijkl, nb_bits, bk);
  resta(aux2, j2k2, aux1, nb_bits, bk);
  sum(aux1, aux2, i2l2, nb_bits, bk);

  resta(aux2, jl2n, k2ln, nb_bits, bk);
  resta(aux3, aux2, j3l, nb_bits, bk);
  sum(aux2, aux3, j2k2, nb_bits, bk);
  divide_float(r, aux1, aux2, float_bits, nb_bits, bk);

  // c = c / (1 - r)
  resta(aux1, uno, r, nb_bits, bk);
  divide_float(aux2, c, aux1, float_bits, nb_bits, bk);
  for(int ci=0; ci < nb_bits; ci++)
    bootsCOPY(&c[ci], &aux2[ci], bk);


  /*
    b = (vl - kw + cjk - cil)/(jl - k2)
  */
  LweSample* cjk = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* cil = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  multiply_float(cjk, c, jk, float_bits, nb_bits, bk);
  multiply_float(cil, c, jk, float_bits, nb_bits, bk);
  resta(aux1, cjk, cil, nb_bits, bk);
  resta(aux2, vl, kw, nb_bits, bk);
  sum(aux3, aux1, aux2, nb_bits, bk);
  resta(aux1, jl, k2, nb_bits, bk);
  divide_float(b, aux3, aux1, float_bits, nb_bits, bk);

  /*
    a = (w - (bk + cj)) / l
  */
  multiply_float(aux1, b, k, float_bits, nb_bits, bk);
  multiply_float(aux2, c, j, float_bits, nb_bits, bk);
  sum(aux3, aux1, aux2, nb_bits, bk);
  resta(aux1, w, aux3, nb_bits, bk);
  divide_float(a, aux1, l, float_bits, nb_bits, bk);

}
