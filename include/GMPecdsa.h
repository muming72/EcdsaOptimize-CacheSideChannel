#pragma once
#include"GMPell.h"

static unsigned long int rand_seed = 114;
void HashToMpz(const char* m, mpz_t E);
class SignGen
{
private:	
	void get_random(mpz_t random);
public:
	EllPoint P;
	EllPoint Q;
	mpz_t d;
	mpz_t r;
	mpz_t s;
	SignGen();
	SignGen(const char* m);
	~SignGen();
	void key_pair_gen();
	void Ecdsa_sign_gen(const char* m);
	bool key_vali();
	void print();
};

class SignVerify
{
private:
public:
	static bool Ecdsa_sign_verify(EllPoint* Q, const char* m, mpz_t r, mpz_t s);
};




