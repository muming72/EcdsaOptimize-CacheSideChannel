#pragma once
#include"Init.h"
using namespace std;
#define ModN 1
#define ModP 0
extern pthread_mutex_t mutex;// = PTHREAD_MUTEX_INITIALIZER;
extern pthread_cond_t  cond;// = PTHREAD_COND_INITIALIZER;
extern bool alternate;// = 1;

void mpz_printf(mpz_t op);
void Fast_mod256(mpz_t rop, mpz_t op);
void Barrett_modN(mpz_t rop,mpz_t op);
void Fp_mod(mpz_t rop,mpz_t op,int modnum);
inline void Fp_Mul(mpz_t rop, mpz_t op1, mpz_t op2);
void Fp_MulN(mpz_t rop, mpz_t op1, mpz_t op2);
inline void Fp_Add(mpz_t rop, mpz_t op1, mpz_t op2);
inline void Fp_Sub(mpz_t rop, mpz_t op1, mpz_t op2);
void Mpz2wNAF(mpz_t k, vector<long int>& wNaf, int w);

class EllPoint
{
private:
	void FixedMul(mpz_t k);
	void wNafMul(mpz_t k, EllPoint* op);
public:
	mpz_t x;
	mpz_t y;
	mpz_t jx;
	mpz_t jy;
	mpz_t jz;
	EllPoint();
	EllPoint(mpz_t x, mpz_t y);
	EllPoint(mpz_t x, mpz_t y, mpz_t z);
	~EllPoint();
	void Setp(mpz_t x, mpz_t y);
	void Setp(mpz_t x, mpz_t y, mpz_t z);
	void Setp(EllPoint* op);
	bool Is_inf();
	bool Is_in_ell();
	void print();
	void Pdouble(EllPoint* rop);
	void Add(EllPoint* rop, EllPoint* op);
	void Sub(EllPoint* p, EllPoint* q);
	void Mul(mpz_t k, EllPoint* op);
	void MulP(mpz_t k, EllPoint* op);
	void MP_Mul(mpz_t k,mpz_t l,EllPoint *P,EllPoint* Q);
	void SpyMul(mpz_t k, EllPoint* op);
};
class FixedPoint
{
public:
	EllPoint* FixedP[Fixedlen];
	FixedPoint()
	{
		FixedP[0] = new EllPoint(P256Para.x, P256Para.y);
		for (int i = 1; i < Fixedlen; i++)
		{
			FixedP[i] = new EllPoint;
			FixedP[i]->Pdouble(FixedP[i - 1]);
		}
	}
	~FixedPoint()
	{
		for (int i = 1; i < 256; i++)
		{
			delete FixedP[i];
		}
	}
};
extern FixedPoint FixPoint;


