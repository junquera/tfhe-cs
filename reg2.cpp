#include "funcions.h"

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

void regresion_cuadratica(LweSample* a, LweSample* b, LweSample* c, List<LweSample*> xs, List<LweSample*> ys, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){

  LweSample* i = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* j = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* k = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* l = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* u = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* v = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* w = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  int n = xs.size();
  if(xs.size() != ys.size()){
    throw "regresion_cuadratica: len(xs) != len(ys)";
  }


  // Inicializando variables
  for(int ci=0; ci<nbc_bits; ci++){
    bootsCONSTANT(&i[ci], 0, bk);
    bootsCONSTANT(&j[ci], 0, bk);
    bootsCONSTANT(&k[ci], 0, bk);
    bootsCONSTANT(&l[ci], 0, bk);
    bootsCONSTANT(&u[ci], 0, bk);
    bootsCONSTANT(&v[ci], 0, bk);
    bootsCONSTANT(&w[ci], 0, bk);
  }

  for(int ci=0 ; ci < n; ci++){
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

  }

  LweSample* 2ijkl;
  LweSample* il;
  LweSample* i2l2;
  LweSample* iklw;
  LweSample* il2v;
  LweSample* j2;
  LweSample* j2k2;
  LweSample* jk;
  LweSample* jk2v;
  LweSample* jklv;
  LweSample* jl;
  LweSample* j3l;
  LweSample* jl2n;
  LweSample* k2;
  LweSample* k2ln;
  LweSample* kw;
  LweSample* ln;
  LweSample* ul;
  LweSample* vl;
  LweSample* w;
  LweSample* wj;


}
