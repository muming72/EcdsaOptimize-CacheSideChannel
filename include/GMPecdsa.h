#pragma once
#include"GMPell.h"
#if MODE_ == GMP_ 
#include<openssl/sha.h>
#include<time.h>
static unsigned long int rand_seed = 114;

class SignGen
{
private:
public:
	EllPoint P;
	EllPoint Q;
	mpz_t d;
	mpz_t r;
	mpz_t s;
	SignGen();
	SignGen(const char* m);
	~SignGen();
	void get_random(mpz_t random);
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

void HashToMpz(const char* m, mpz_t E);

#endif


