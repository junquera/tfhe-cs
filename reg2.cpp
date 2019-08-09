#include "functions.h"
#include <vector>
using namespace std;
/*

i = 0
j = 0
k = 0
l = 0

u = 0
v = 0
w = 0

n = len(xys)

for x, y in xys:
    i += x
    j += x**2
    k += x**3
    l += x**4

    u += y
    v += (x * y)
    w += ((x**2) * y)


print("i", i)
print("j", j)
print("k", k)
print("l", l)
print("u", u)
print("v", v)
print("w", w)

c = (u*l - w*j)/(l*n - j**2)
c += (j*k*l*v - j*(k**2)*w - i*(l**2)*v + i*k*l*w)/(j*(l**2)*n - (k**2)*l*n - (j**3)*l + (j**2)*(k**2))
r = ((j**2)*(k**2)-2*(i*j*k*l)+(i**2)*(l**2))/(j*(l**2)*n - (k**2)*l*n - (j**3)*l + (j**2)*(k**2))
c = c / (1 - r)

b = (v*l - k*w + c*j*k - c*i*l)/(j*l - k**2)
a = (w - b*k - c*j) / l

*/

void regresion_cuadratica(LweSample* a, LweSample* b, LweSample* c, const vector<LweSample*> xs, const vector<LweSample*> ys, const int float_bits, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){

  LweSample* aux1 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux3 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux4 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* n = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* uno = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* i = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* j = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* k = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* l = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* u = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* v = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* w = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  int values = xs.size();
  if(xs.size() != ys.size()){
    throw "regresion_cuadratica: len(xs) != len(ys)";
  }

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
    bootsCONSTANT(&n[ci], (values>>ci)&1, bk);
    bootsCONSTANT(&uno[ci], 0, bk);
  }
  bootsCONSTANT(&uno[0], 1, bk);

  // Ajustando a float_bits
  shiftl(aux1, n, float_bits, nb_bits, bk);
  shiftl(aux2, uno, float_bits, nb_bits, bk);
  for(int ci=0; ci < nb_bits; ci++){
    bootsCOPY(&n[ci], &aux1[ci], bk);
    bootsCOPY(&uno[ci], &aux2[ci], bk);
  }

  cout << "Variables inicializadas" << endl;

  for(int ci=0 ; ci < values; ci++){
    LweSample* x = xs[ci];
    LweSample* y = ys[ci];

    LweSample* x2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
    LweSample* x3 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
    LweSample* x4 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

    multiply(x2, x, x, nb_bits, bk);
    multiply(x3, x2, x, nb_bits, bk);
    multiply(x4, x2, x2, nb_bits, bk);

    LweSample* xy = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
    LweSample* x2y = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

    multiply(xy, x, y, nb_bits, bk);
    multiply(x2y, x2, y, nb_bits, bk);

    cout << "xys" << ci << endl;
  }

  LweSample* i2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* j2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* k2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* l2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  h_pow(i2, k, 2, nb_bits, bk);
  h_pow(j2, j, 2, nb_bits, bk);
  h_pow(k2, k, 2, nb_bits, bk);
  h_pow(l2, k, 2, nb_bits, bk);

  cout << "Cuadrados i,j,k,l" << endl;

  LweSample* il = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* jk = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* jl = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* kw = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* kv = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* ln = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* ul = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* vl = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* wj = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  multiply(il, i, l, nb_bits, bk);
  multiply(jk, j, k, nb_bits, bk);
  multiply(jl, j, l, nb_bits, bk);
  multiply(kw, k, w, nb_bits, bk);
  multiply(kv, k, v, nb_bits, bk);
  multiply(ln, l, n, nb_bits, bk);
  multiply(ul, u, l, nb_bits, bk);
  multiply(vl, v, l, nb_bits, bk);
  multiply(wj, w, j, nb_bits, bk);

  cout << "Duplas" << endl;

  LweSample* ijkl = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* i2l2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* iklw = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* il2v = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* j2k2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* jk2w = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* jklv = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* j3l = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* jl2n = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* k2ln = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

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

  cout << "Complejos" << endl;

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

  cout << "Pre r" << endl;

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

  cout << "c" << endl;

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

  cout << "b" << endl;

  /*
    a = (w - (bk + cj)) / l
  */
  multiply_float(aux1, b, k, float_bits, nb_bits, bk);
  multiply_float(aux2, c, j, float_bits, nb_bits, bk);
  suma(aux3, aux1, aux2, nb_bits, bk);
  resta(aux1, w, aux3, nb_bits, bk);
  divide_float(a, aux1, l, float_bits, nb_bits, bk);

  cout << "a" << endl;

}
