#ifndef _CLIENT_H_INCLUDED_
#define _CLIENT_H_INCLUDED_

#include "aux.h"
#include "reg2.h"

class HClient {
	public:
		HClient(int nb_bits, int float_bits);
		void cifra(LweSample* answer, int32_t input);
		int32_t descifra(LweSample* answer);
		void exportSecretKeyToFile(string name);
		void exportCloudKeyToFile(string name);
		void getCloudKey(TFheGateBootstrappingCloudKeySet* &cloudKey);
	private:
		int nb_bits;
		int float_bits;
		TFheGateBootstrappingSecretKeySet* key;
};

#endif
