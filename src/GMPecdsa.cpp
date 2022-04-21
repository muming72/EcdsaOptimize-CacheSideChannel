#include"GMPecdsa.h"

SignGen::SignGen()
{
	P.Setp(P256Para.x, P256Para.y);
	mpz_init(d);
	mpz_init(r);
	mpz_init(s);
	key_pair_gen();

}
SignGen::SignGen(const char* m)
{
	P.Setp(P256Para.x,P256Para.y);
	mpz_init(d);
	mpz_init(r);
	mpz_init(s);
	key_pair_gen();
	Ecdsa_sign_gen(m);
}
SignGen::~SignGen()
{
	mpz_clear(d);
	mpz_clear(r);
	mpz_clear(s);
}

void SignGen::get_random(mpz_t random)
{
	time_t t;
	t = time(&t);
	srand(rand_seed+t);
	rand_seed = rand();
	gmp_randstate_t grt;
	gmp_randinit_default(grt);
	gmp_randseed_ui(grt, rand_seed);
	mpz_urandomm(random, grt,P256Para.n);
	//mpz_set_str(random, "2841672385478624782973482364872634928374823", 16);
}

bool SignGen::key_vali()
{
	if (Q.Is_inf())
	{
		return false;
	}
	if (mpz_cmp(Q.x,P256Para.p)>=0|| mpz_cmp(Q.y, P256Para.p) >=0)
	{
		return false;
	}
//	if (!Q.Is_in_ell())
//	{
//		return false;
//	}
	//EllPoint nQ;
	//nQ.Mul(P256Para.n, &Q);
	//if (nQ.Is_inf())
	//{
	//	return true;
//	}
	return true;//false;
}
void SignGen::key_pair_gen()
{
	do
	{ 
		get_random(d);
		Q.MulP(d, &P);
	} while (!key_vali());
	//mpz_printf(d);
}
void SignGen::Ecdsa_sign_gen( const char* m)
{
	mpz_t k, E, dr, k_;
	mpz_init(k);
	mpz_init(E);
	mpz_init(dr);
	mpz_init(k_);
	EllPoint q;
	do{	
		get_random(k);
		q.MulP(k, &P);
		mpz_set(r, q.x);
		mpz_mod(r, r, P256Para.n);
	} while (mpz_cmp_si(r,0)==0);


	HashToMpz(m, E);
	mpz_invert(k_, k, P256Para.n);
	Fp_MulN(dr, d, r);
	mpz_add(E,E,dr);
	if(mpz_cmp(E,P256Para.n)>=0)
	{
		mpz_sub(E,E,P256Para.n);
	}
	Fp_MulN(s, k_, E);

	mpz_clear(k);
	mpz_clear(k_);
	mpz_clear(dr);
	mpz_clear(E);
}

void SignGen::print()
{
	printf("Public key Q:\n");
	Q.print();
	gmp_printf("Private key d:\n%Zd\n\n", d);
	gmp_printf("Signature:\nr:%Zd\ns:%Zd\n", r,s);
}

bool SignVerify::Ecdsa_sign_verify(EllPoint* Q, const char* m, mpz_t r, mpz_t s)
{
	
	mpz_t E, u1, u2, w;
	EllPoint p1, q1,P;
	P.Setp(P256Para.x, P256Para.y);
	mpz_init(E);
	mpz_init(u1);
	mpz_init(u2);
	mpz_init(w);
	
	if (mpz_cmp(r,P256Para.n)>=0|| mpz_cmp(s, P256Para.n) >= 0)
	{
		printf("N error\n");
		return false;
	}
	
	HashToMpz(m, E);
	mpz_invert(w, s, P256Para.n);

	Fp_MulN(u1, E, w);
	Fp_MulN(u2, r, w);
	if(Cont.Muti){	
		p1.MP_Mul(u1,u2,&P,Q);
	}
	else{
		p1.MulP(u1, &P);q1.Mul(u2, Q);p1.Add(&p1, &q1);	
	}
	
	if (p1.Is_inf())
	{
		mpz_clear(E);
		mpz_clear(w);
		mpz_clear(u1);
		mpz_clear(u2);
		printf("inf error\n");
		return false;
	}
	mpz_set(u1, p1.x);
	mpz_mod(u1,u1, P256Para.n);
//	mpz_printf(u1);
//	mpz_printf(r);
	if (mpz_cmp(u1,r) == 0)
	{
		mpz_clear(E);
		mpz_clear(w);
		mpz_clear(u1);
		mpz_clear(u2);
		//printf("seccess\n\n");
		return true;
	}
	mpz_clear(E);
	mpz_clear(w);
	mpz_clear(u1);
	mpz_clear(u2);
	printf("uv error\n");
	return false;
}

void HashToMpz(const char* m, mpz_t E)
{
	
	unsigned char buf[ParaSize_/8];
#if Para_ == 192
	SHA1((unsigned char*)m, strlen(m), buf);
#elif Para_ == 224
	SHA224((unsigned char*)m, strlen(m), buf);
#elif Para_ == 256
	SHA256((unsigned char*)m, strlen(m), buf);
#elif Para_ == 384
	SHA384((unsigned char*)m, strlen(m), buf);
#elif Para_ == 521
	SHA512((unsigned char*)m, strlen(m), buf);
#endif // Para384_
	mpz_import(E,ParaSize_/8,1,1,0,0,buf);
}
