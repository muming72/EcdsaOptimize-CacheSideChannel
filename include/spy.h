#include"GMPecdsa.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
//typedef void (EllPoint::*PMul)(mpz_t k,EllPoint* op);
//typedef void (EllPoint::*PAdd)(EllPoint* p, EllPoint* q);
//typedef void (EllPoint::*PDoub)(EllPoint* rop);
static unsigned long long int flush_reload_threshold;

inline void maceess(void* addr);
inline unsigned long long int rdtsc();
unsigned long long int get_FlushReload_time();
inline void flush(void* addr);
bool flush_reload(void* addr);

void Spytest();

class unsafe_sign: public SignGen{
public:
    void Ecdsa_sign_gen(const char* m);
};
class unsafe_ellpoint: public EllPoint{
public:
    void unsafe_MulP(mpz_t k,EllPoint* op);
};

class Safe_sign: public SignGen{
public:
    void Ecdsa_sign_gen(const char* m);
};
class Safe_ellpoint: public EllPoint{
public:
    void safe_MulP(mpz_t k,EllPoint* op);
};