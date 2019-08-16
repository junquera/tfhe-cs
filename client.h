#ifndef _CLIENT_H_INCLUDED_
#define _CLIENT_H_INCLUDED_

#include "common/hefile.h"
#include "reg2.h"


class HClient {
	public:
		HClient(int _nb_bits, int _float_bits);
		void genKeys();
		void setKeysFromFile(string keyFileName);
		void cifra(LweSample* answer, int64_t input);
		int64_t descifra(LweSample* answer);
		void exportSecretKeyToFile(string name);
		void exportCloudKeyToFile(string name);
		void getCloudKey(TFheGateBootstrappingCloudKeySet* &cloudKey);
	private:
		int nb_bits;
		int float_bits;
		TFheGateBootstrappingSecretKeySet* key;
};

#endif
