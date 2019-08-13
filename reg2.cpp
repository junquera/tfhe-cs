#include "aux.h"
#include "reg2.h"

RegresionCuadratica::RegresionCuadratica(int _nb_bits, int _float_bits, TFheGateBootstrappingCloudKeySet* _bk) {

  nb_bits = _nb_bits;
  float_bits = _float_bits;

  bk = _bk;

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
    bootsCONSTANT(&aux1[ci], 0, bk);
    bootsCONSTANT(&aux2[ci], 0, bk);
    bootsCONSTANT(&aux3[ci], 0, bk);
    bootsCONSTANT(&aux4[ci], 0, bk);
    bootsCONSTANT(&n[ci], 0, bk);
    bootsCONSTANT(&uno[ci], 0, bk);
    bootsCONSTANT(&i[ci], 0, bk);
    bootsCONSTANT(&j[ci], 0, bk);
    bootsCONSTANT(&k[ci], 0, bk);
    bootsCONSTANT(&l[ci], 0, bk);
    bootsCONSTANT(&u[ci], 0, bk);
    bootsCONSTANT(&v[ci], 0, bk);
    bootsCONSTANT(&w[ci], 0, bk);
    bootsCONSTANT(&i2[ci], 0, bk);
    bootsCONSTANT(&j2[ci], 0, bk);
    bootsCONSTANT(&k2[ci], 0, bk);
    bootsCONSTANT(&l2[ci], 0, bk);
    bootsCONSTANT(&il[ci], 0, bk);
    bootsCONSTANT(&jk[ci], 0, bk);
    bootsCONSTANT(&jl[ci], 0, bk);
    bootsCONSTANT(&kw[ci], 0, bk);
    bootsCONSTANT(&kv[ci], 0, bk);
    bootsCONSTANT(&ln[ci], 0, bk);
    bootsCONSTANT(&ul[ci], 0, bk);
    bootsCONSTANT(&vl[ci], 0, bk);
    bootsCONSTANT(&wj[ci], 0, bk);
    bootsCONSTANT(&ijkl[ci], 0, bk);
    bootsCONSTANT(&i2l2[ci], 0, bk);
    bootsCONSTANT(&iklw[ci], 0, bk);
    bootsCONSTANT(&il2v[ci], 0, bk);
    bootsCONSTANT(&j2k2[ci], 0, bk);
    bootsCONSTANT(&jk2w[ci], 0, bk);
    bootsCONSTANT(&jklv[ci], 0, bk);
    bootsCONSTANT(&j3l[ci], 0, bk);
    bootsCONSTANT(&jl2n[ci], 0, bk);
    bootsCONSTANT(&k2ln[ci], 0, bk);
  }

  // Inicializando variables
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

/*
  multi = 5
  suma = 7

  t_multi = 93
  t_suma = 6

  t_total = (t_multi*multi + t_suma*suma)*values
          = (465 + 42)*values = (507*values)s = (8.45*values) minutos
*/
void RegresionCuadratica::initVectores(const vector<LweSample*> xs, const vector<LweSample*> ys, string results_path){

  int values = xs.size();
  if(xs.size() != ys.size()){
    throw "regresion_cuadratica: len(xs) != len(ys)";
  }

  // Inicializa "n"
  for(int ci=0; ci < nb_bits; ci++)
    bootsCONSTANT(&n[ci], (values>>ci)&1, bk);

  // Ajusta "n" a float_bits
  shiftl(aux1, n, float_bits, nb_bits, bk);
  for(int ci=0; ci < nb_bits; ci++)
    bootsCOPY(&n[ci], &aux1[ci], bk);

  bool exists_i = retrieveResult(results_path + "/" + "i", i, nb_bits, bk->params);
  bool exists_j = retrieveResult(results_path + "/" + "j", j, nb_bits, bk->params);
  bool exists_k = retrieveResult(results_path + "/" + "k", k, nb_bits, bk->params);
  bool exists_l = retrieveResult(results_path + "/" + "l", l, nb_bits, bk->params);

  bool exists_u = retrieveResult(results_path + "/" + "u", u, nb_bits, bk->params);
  bool exists_v = retrieveResult(results_path + "/" + "v", v, nb_bits, bk->params);
  bool exists_w = retrieveResult(results_path + "/" + "w", w, nb_bits, bk->params);

  if(exists_i & exists_j & exists_k & exists_l & exists_u & exists_v & exists_w) return;

  // values = 2; // Para las pruebas
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
    sum(aux2, j, x2, nb_bits, bk);
    sum(aux3, k, x3, nb_bits, bk);
    sum(aux4, l, x4, nb_bits, bk);

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
    cout << "Init vector: " << ci << endl;

    delete_gate_bootstrapping_ciphertext_array(nb_bits, x);
    delete_gate_bootstrapping_ciphertext_array(nb_bits, x2);
    delete_gate_bootstrapping_ciphertext_array(nb_bits, x3);
    delete_gate_bootstrapping_ciphertext_array(nb_bits, x4);
    delete_gate_bootstrapping_ciphertext_array(nb_bits, y);
    delete_gate_bootstrapping_ciphertext_array(nb_bits, xy);
    delete_gate_bootstrapping_ciphertext_array(nb_bits, x2y);

  }

  saveResult(results_path + "/" + "i", i, nb_bits, bk->params);
  saveResult(results_path + "/" + "j", j, nb_bits, bk->params);
  saveResult(results_path + "/" + "k", k, nb_bits, bk->params);
  saveResult(results_path + "/" + "l", l, nb_bits, bk->params);

  saveResult(results_path + "/" + "u", u, nb_bits, bk->params);
  saveResult(results_path + "/" + "v", v, nb_bits, bk->params);
  saveResult(results_path + "/" + "w", w, nb_bits, bk->params);
};

/*
  multi = 4

  t_multi = 93

  t_total = t_multi*multi = 372s = 6.2 minutos
*/
void RegresionCuadratica::calcCuadrados(string results_path){

  bool exists_i2 = retrieveResult(results_path + "/" + "i2", i2, nb_bits, bk->params);
  bool exists_j2 = retrieveResult(results_path + "/" + "j2", j2, nb_bits, bk->params);
  bool exists_k2 = retrieveResult(results_path + "/" + "k2", k2, nb_bits, bk->params);
  bool exists_l2 = retrieveResult(results_path + "/" + "l2", l2, nb_bits, bk->params);

  if(exists_i2 & exists_j2 & exists_k2 & exists_l2) return;

  multiply_float(i2, i, i, float_bits, nb_bits, bk);
  multiply_float(j2, j, j, float_bits, nb_bits, bk);
  multiply_float(k2, k, k, float_bits, nb_bits, bk);
  multiply_float(l2, k, k, float_bits, nb_bits, bk);

  saveResult(results_path + "/" + "i2", i2, nb_bits, bk->params);
  saveResult(results_path + "/" + "j2", j2, nb_bits, bk->params);
  saveResult(results_path + "/" + "k2", k2, nb_bits, bk->params);
  saveResult(results_path + "/" + "l2", l2, nb_bits, bk->params);
};

/*
  multi = 9

  t_multi = 93

  t_total = t_multi*multi = 837s = 13.9 minutos
*/
void RegresionCuadratica::calcDuplas(string results_path){

  bool exists_il = retrieveResult(results_path + "/" + "il", il, nb_bits, bk->params);
  bool exists_jk = retrieveResult(results_path + "/" + "jk", jk, nb_bits, bk->params);
  bool exists_jl = retrieveResult(results_path + "/" + "jl", jl, nb_bits, bk->params);
  bool exists_kw = retrieveResult(results_path + "/" + "kw", kw, nb_bits, bk->params);
  bool exists_kv = retrieveResult(results_path + "/" + "kv", kv, nb_bits, bk->params);
  bool exists_ln = retrieveResult(results_path + "/" + "ln", ln, nb_bits, bk->params);
  bool exists_ul = retrieveResult(results_path + "/" + "ul", ul, nb_bits, bk->params);
  bool exists_vl = retrieveResult(results_path + "/" + "vl", vl, nb_bits, bk->params);
  bool exists_wj = retrieveResult(results_path + "/" + "wj", wj, nb_bits, bk->params);

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

  saveResult(results_path + "/" + "il", il, nb_bits, bk->params);
  saveResult(results_path + "/" + "jk", jk, nb_bits, bk->params);
  saveResult(results_path + "/" + "jl", jl, nb_bits, bk->params);
  saveResult(results_path + "/" + "kw", kw, nb_bits, bk->params);
  saveResult(results_path + "/" + "kv", kv, nb_bits, bk->params);
  saveResult(results_path + "/" + "ln", ln, nb_bits, bk->params);
  saveResult(results_path + "/" + "ul", ul, nb_bits, bk->params);
  saveResult(results_path + "/" + "vl", vl, nb_bits, bk->params);
  saveResult(results_path + "/" + "wj", wj, nb_bits, bk->params);
};

/*
  multi = 10

  t_multi = 93

  t_total = t_multi*multi = 930s = 15.5 minutos
*/
void RegresionCuadratica::calcComplejos(string results_path){

  bool exists_ijkl = retrieveResult(results_path + "/" + "ijkl", ijkl, nb_bits, bk->params);
  bool exists_i2l2 = retrieveResult(results_path + "/" + "i2l2", i2l2, nb_bits, bk->params);
  bool exists_iklw = retrieveResult(results_path + "/" + "iklw", iklw, nb_bits, bk->params);
  bool exists_il2v = retrieveResult(results_path + "/" + "il2v", il2v, nb_bits, bk->params);
  bool exists_j2k2 = retrieveResult(results_path + "/" + "j2k2", j2k2, nb_bits, bk->params);
  bool exists_jk2w = retrieveResult(results_path + "/" + "jk2w", jk2w, nb_bits, bk->params);
  bool exists_jklv = retrieveResult(results_path + "/" + "jklv", jklv, nb_bits, bk->params);
  bool exists_j3l = retrieveResult(results_path + "/" + "j3l", j3l, nb_bits, bk->params);
  bool exists_jl2n = retrieveResult(results_path + "/" + "jl2n", jl2n, nb_bits, bk->params);
  bool exists_k2ln = retrieveResult(results_path + "/" + "k2ln", k2ln, nb_bits, bk->params);


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

  saveResult(results_path + "/" + "ijkl", ijkl, nb_bits, bk->params);
  saveResult(results_path + "/" + "i2l2", i2l2, nb_bits, bk->params);
  saveResult(results_path + "/" + "iklw", iklw, nb_bits, bk->params);
  saveResult(results_path + "/" + "il2v", il2v, nb_bits, bk->params);
  saveResult(results_path + "/" + "j2k2", j2k2, nb_bits, bk->params);
  saveResult(results_path + "/" + "jk2w", jk2w, nb_bits, bk->params);
  saveResult(results_path + "/" + "jklv", jklv, nb_bits, bk->params);
  saveResult(results_path + "/" + "j3l", j3l, nb_bits, bk->params);
  saveResult(results_path + "/" + "jl2n", jl2n, nb_bits, bk->params);
  saveResult(results_path + "/" + "k2ln", k2ln, nb_bits, bk->params);

};

void RegresionCuadratica::calcula(LweSample* a, LweSample* b, LweSample* c, const vector<LweSample*> xs, const vector<LweSample*> ys, string results_path){

  time_t t0 = time(NULL);
  initVectores(xs, ys, results_path);
  cout << "initVectores! " << time(NULL) - t0 << endl;
  calcCuadrados(results_path);
  cout << "calcCuadrados! " << time(NULL) - t0 << endl;
  calcDuplas(results_path);
  cout << "calcDuplas! " << time(NULL) - t0 << endl;
  calcComplejos(results_path);
  cout << "calcComplejos! " << time(NULL) - t0 << endl;

  for(int ci=0; ci < nb_bits; ci++){
    bootsCONSTANT(&a[ci], 0, bk);
    bootsCONSTANT(&b[ci], 0, bk);
    bootsCONSTANT(&c[ci], 0, bk);
  }
  calcC(c, results_path);
  cout << "CalcC! " << time(NULL) - t0 << endl;
  calcB(b, c, results_path);
  cout << "CalcB! " << time(NULL) - t0 << endl;
  calcA(a, b, c, results_path);
  cout << "CalcA! " << time(NULL) - t0 << endl;

};

/*
  suma = 16
  div = 4

  t_suma = 6
  t_div = 1386

  t_total = t_div*div + t_suma*suma
          = 5944 + 96 = 6040s = 1.7 horas
*/
void RegresionCuadratica::calcC(LweSample* c, string results_path){
  /*
  c = (ul - wj)/(ln - j2)
  c += (jklv - jk2w - il2v + iklw)/(jl2n - k2ln - j3l + j2k2)
  r = (j2k2-2(ijkl)+i2l2)/(jl2n - (k2ln - j3l + j2k2))
  c = c / (1 - r)
  */
  bool exists_c = retrieveResult(results_path + "/" + "c", c, nb_bits, bk->params);
  if (exists_c)
      return;


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

  saveResult(results_path + "/" + "c", c, nb_bits, bk->params);

};

/*
  suma = 4
  multi = 2
  div = 1

  t_suma = 6
  t_multi = 93
  t_div = 1386

  t_total = t_div*div + t_multi*multi + t_suma*suma
          = 1386 + 186 + 24 = 1596s = 26.6 minutos
*/
void RegresionCuadratica::calcB(LweSample* b, LweSample* c, string results_path){

  bool exists_b = retrieveResult(results_path + "/" + "b", b, nb_bits, bk->params);
  if (exists_b)
      return;

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

  saveResult(results_path + "/" + "b", b, nb_bits, bk->params);

};

/*
  suma = 2
  multi = 2
  div = 1

  t_suma = 6
  t_multi = 93
  t_div = 1386

  t_total = t_div*div + t_multi*multi + t_suma*suma
          = 1386 + 186 + 12 = 1584s = 26.4 minutos
*/
void RegresionCuadratica::calcA(LweSample* a, LweSample* b, LweSample* c, string results_path){

  /*
  a = (w - (bk + cj)) / l
  */
  bool exists_a = retrieveResult(results_path + "/" + "a", a, nb_bits, bk->params);
  if (exists_a)
      return;

  multiply_float(aux1, b, k, float_bits, nb_bits, bk);
  multiply_float(aux2, c, j, float_bits, nb_bits, bk);
  sum(aux3, aux1, aux2, nb_bits, bk);
  resta(aux1, w, aux3, nb_bits, bk);
  divide_float(a, aux1, l, float_bits, nb_bits, bk);

  saveResult(results_path + "/" + "a", a, nb_bits, bk->params);

};
