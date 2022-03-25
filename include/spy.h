#include"GMPecdsa.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
//typedef void (EllPoint::*PMul)(mpz_t k,EllPoint* op);
//typedef void (EllPoint::*PAdd)(EllPoint* p, EllPoint* q);
//typedef void (EllPoint::*PDoub)(EllPoint* rop);
//static int Stop_spy=0;
static char key[512];
static unsigned long long int flush_reload_threshold = 250;

inline void maceess(void* addr);
inline unsigned long long int rdtsc();
unsigned long long int get_FlushReload_time();
inline void flush(void* addr);
bool flush_reload(void* addr);
void* spyth(void* arg);
void Spytest();
void* signth(void* arg);