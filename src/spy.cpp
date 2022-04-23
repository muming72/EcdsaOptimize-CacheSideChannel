#include"spy.h"
static char key[512];

void* addr_Padd = (void*)&EllPoint::Add;
#define  Message "information security"

inline void maceess(void* addr)
{
	asm volatile("mfence");
	asm volatile("movl (%0),%%eax \n" : :"c"(addr):"%eax");
	asm volatile("mfence");
}
inline void flush(void* addr)
{
	asm volatile("mfence");
	asm volatile("clflush 0(%0) \n" : :"c"(addr));
	asm volatile("mfence");

}
inline unsigned long long int rdtsc()
{
	unsigned long long int a=0;
	asm volatile("mfence");
	asm volatile("rdtsc" : "=A"(a));
	asm volatile("mfence");
	return a;
}
unsigned long long int get_FlushReload_time()
{
	unsigned long long int start,end,reload_time,flush_time;
	reload_time=0;
	flush_time=0;
	int a[16];
	void* addr=addr_Padd;
	unsigned long long int count =10000;
	maceess(addr);
	for(int i=0;i<count;i++)
	{
		start = rdtsc();
		maceess(addr);
		end = rdtsc();
		reload_time +=(end - start);
	}
	for(int i=0;i<count;i++)
	{
		flush(addr);
		start = rdtsc();
		maceess(addr);
		end = rdtsc();
		flush_time +=(end - start);
	}
    flush(addr);
	reload_time /=count;
	flush_time/=count;
	printf("reload:%llu,flush:%llu\n",reload_time,flush_time);
	return (reload_time*2+flush_time)/3;
}
bool __attribute((always_inline)) flush_reload(void* addr)
{
    unsigned long long int start,end;
    start = rdtsc();
	maceess(addr);
	end = rdtsc();
    flush(addr);
    printf("%llu\n",end-start);
    if((end-start)<flush_reload_threshold)
    {
        return 1;
    }
    return 0;
}
int numberaddr;void* addr = &numberaddr;
void unsafe_sign::Ecdsa_sign_gen(const char* m)
{
	mpz_t k, E, dr, k_;
	mpz_init(k);
	mpz_init(E);
	mpz_init(dr);
	mpz_init(k_);
	unsafe_ellpoint q;
	do{	
		get_random(k);
		q.unsafe_MulP(k,&P);
		
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
void unsafe_ellpoint::unsafe_MulP(mpz_t k,EllPoint* op)
{	
	addr = addr_Padd;
	unsigned long long int start;
	unsigned long long int end;
	EllPoint inf;
	mpz_printf(k);
	int size = mpz_sizeinbase(k, 2);
	printf("%d",size);
	for (int i = size - 1; i >= 0; i--)
	{
		inf.Pdouble(&inf);
		if (mpz_tstbit(k, i))
		{
			inf.Add(&inf, op);
			//maceess(addr);
			flush((void*)addr);
		}
		start = rdtsc();
		maceess(addr);
		end = rdtsc();
    	flush(addr);
		//bool a = flush_reload(addr_Padd);
		//printf("%llu\n",end-start);
		if((end-start)<flush_reload_threshold)
    	{
        	//rintf("1 ");
			key[size- 1-i]='0';
    	}
		else
		{
			//printf("0 ");
			key[size- 1-i]='1';
		}
		//flush((void*)addr_Padd);
	}
	key[size] = 0;
	Setp(&inf);
}

void Safe_sign::Ecdsa_sign_gen(const char* m)
{
	mpz_t k, E, dr, k_;
	mpz_init(k);
	mpz_init(E);
	mpz_init(dr);
	mpz_init(k_);
	Safe_ellpoint q;
	do{	
		get_random(k);
		q.safe_MulP(k,&P);
		
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
void Safe_ellpoint::safe_MulP(mpz_t k,EllPoint* op){
	if (op->Is_inf())
	{
		Setp(op);
		return;
	}
	EllPoint op1;
	EllPoint op2(op->x,op->y);
	for (int i = mpz_sizeinbase(k, 2) - 1; i >= 0; i--)
	{
		if (mpz_tstbit(k, i))
		{
			op1.Add(&op1,&op2);
			op2.Pdouble(&op2);
		}
		else{
			op2.Add(&op1,&op2);
			op1.Pdouble(&op1);
		}
	}
	Setp(&op1);
}
void Spytest()
{
	flush_reload_threshold = get_FlushReload_time();
	unsafe_sign Sign;
	Sign.Ecdsa_sign_gen(Message);

	printf("\nstr:%s\n",key);
	mpz_t mpz_key;
	mpz_init_set_str(mpz_key,key,2);
	mpz_printf(mpz_key);
}

