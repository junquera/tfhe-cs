#include "server.h"

/**
Operaciones de regresión cuadrática:
- 6       division        1386 segundos/operación
- 32      multiplicacion  93 segundos/operación
- 22      suma/resta      6 segundos/operación

TOTAL: 11424 segundos = 190 minutos = 3.17 horas
*/
HServer::HServer(int _nb_bits, int _float_bits) {
  nb_bits = _nb_bits;
  float_bits = _float_bits;
};

void HServer::regresionCuadratica(LweSample* a, LweSample* b, LweSample* c, const vector<LweSample*> xs, const vector<LweSample*> ys, string cloud_key_path, string results_path) {
  TFheGateBootstrappingCloudKeySet* bk;
  loadCloudKeyFromFile(cloud_key_path, bk);
  RegresionCuadratica rg(nb_bits, float_bits, bk);
  rg.calcula(a, b, c, xs, ys, results_path);
};
