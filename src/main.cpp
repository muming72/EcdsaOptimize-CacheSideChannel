#include"GMPecdsa.h"
#include"spy.h"
#include"cmdline.h"
#define  Message "information security"
int number;
void get_std_curve_name()
{
	EC_builtin_curve* curves = NULL, * p;
	int curves_count, i;
	curves_count = EC_get_builtin_curves(NULL, 0);
	curves = (EC_builtin_curve*)malloc(sizeof(EC_builtin_curve) * curves_count);
	curves_count = EC_get_builtin_curves(curves, curves_count);

	printf("Total built-in EC curves count: %d\n", curves_count);
	printf("Built-in EC curves info:\n");
	p = curves;
	for (i = 0; i < curves_count; i++)
	{
		printf("EC curve item: %d\n", (i + 1));
		printf("NID: %d\n", p->nid);
		printf("Comment: %s\n", p->comment);
		p++;
	}

	free(curves);
}
void testspy()
{
	mpz_t a;
	mpz_init_set_si(a,3);
	flush_reload_threshold = get_FlushReload_time();
	EllPoint Point;
	Point.Setp(P256Para.x,P256Para.y);
	void* p = (void*)a;//&EllPoint::Add;
	flush((void *)p);
	/*
	Point.Add(&Point,&Point);
	printf("%d\n",flush_reload((void *)p));
	flush((void *)p);
	printf("%d\n",flush_reload((void *)p));*/
	char c[20];
	while(1)
	{
		scanf("%s",c);
		if(strcmp((const char*)c,"mul")==0)
		{
			Point.Add(&Point,&Point);
		}
		else if(strcmp((const char*)c,"access")==0)
		{
			maceess((void *)p);
		}
		else if(strcmp((const char*)c,"reload")==0)
		{
			printf("%d\n",flush_reload((void *)p));
			flush(p);
		}
		else if(strcmp((const char*)c,"exit")==0)
		{
			break;
		}
		else if(strcmp((const char*)c,"testtime")==0)
		{
			flush_reload_threshold = get_FlushReload_time();
		}
		else{
			printf("input error\n");
		}
	}
}


void test_openssl()
{
		clock_t start, finish;
		start = clock();
		EC_GROUP* group1;
		EC_KEY* key1;
		key1 = EC_KEY_new();
		group1 = EC_GROUP_new_by_curve_name(714);
		EC_KEY_set_group(key1, group1);
		EC_KEY_generate_key(key1);
		EC_builtin_curve* curves;
		const char* m = Message;
		ECDSA_SIG* s;
		unsigned char buf[32];
		for (int i = 0; i < number; i++)
		{
			//EC_KEY_generate_key(key1);
			SHA256((unsigned char*)m, strlen(m), buf);
			s = ECDSA_do_sign(buf, 32, key1);
			if (ECDSA_do_verify(buf, 32, s, key1))
			{
				//printf("%d\n", EC_GROUP_get_curve_name(group1));
				//EC_KEY_print(a, key1, 13);
			}
		}
		finish = clock();
		double Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
		printf("%f seconds\n", Total_time);
}
void test_security_encrypt()
{
	printf("security\n");
}
void test_spy()
{
	//testspy();
	Spytest();
	printf("spy\n");
}
void test_my_encrypt()
{
	clock_t start, finish;
	start = clock();

	SignGen sign;
	for (int i = 0; i < number; i++)
	{
		//sign->key_pair_gen();
		sign.Ecdsa_sign_gen(Message);
		//sign->print();
		if (SignVerify::Ecdsa_sign_verify(&(sign.Q), Message, sign.r, sign.s))
		{
			//printf("验证成功\n");
		}
	}	
	finish = clock();
	double Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("%f seconds\n", Total_time);
}

void test1(int argc,char** argv)
{
	cmdline::parser a;
	a.add<int>("number",'n',"number of executions",false,500);
	
	a.add("openssl",'o',"use openssl");

	a.add("spy",'\0',"cache side channel");

	a.add("security",'s',"met");

	a.add("wNAF",'w',"window NAF");
	a.add("fixed",'f',"Fixed-base");
	a.add("Bar",'b',"Barrett reduction");
	a.add("Fast",'F',"Fast reduction");
	a.parse_check(argc,argv);
	number = a.get<int>("number");
	if(a.exist("openssl"))
	{
		test_openssl();
	}
	else if(a.exist("spy")){
		test_spy();
	}
	else if(a.exist("security")){
		test_security_encrypt();
	}
	else{
		Cont.wNaf = a.exist("wNAF");
		Cont.Fixed_base_ = a.exist("fixed");
		Cont.Burr_red = a.exist("Bar");
		Cont.Fast_red = a.exist("Fast");
		test_my_encrypt();
	}
}
int main(int argc,char** argv)
{
	test1(argc,argv);

	//testspy();
}



