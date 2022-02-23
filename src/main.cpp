#include"GMPecdsa.h"
#include"spy.h"

#define  Message "information security"

void teststd(EC_KEY* key1)
{
	//BIO* a;
	EC_builtin_curve* curves;
	const char* m = Message;
	ECDSA_SIG* s;
	unsigned char buf[32];
	SHA256((unsigned char*)m, strlen(m), buf);
	s = ECDSA_do_sign(buf, 32, key1);
	if (ECDSA_do_verify(buf, 32, s, key1))
	{
		//printf("%d\n", EC_GROUP_get_curve_name(group1));
		//EC_KEY_print(a, key1, 13);
	}
}
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
void test(SignGen* sign)
{
	for (int i = 0; i < 500; i++)
	{
		//sign->key_pair_gen();
		sign->Ecdsa_sign_gen(Message);
		//sign->print();
		if (SignVerify::Ecdsa_sign_verify(&(sign->Q), Message, sign->r, sign->s))
		{
			//printf("验证成功\n");
		}
	}	
}
void testmpz()
{
	mpz_t a,b;
	mpz_init_set_str(a,"e00d7efba1662985be9403cb055c75d4f7e0ce8d84a9c5114abcaf3",16);
	mpz_init(b);

	clock_t start, finish;
	start = clock();
for(int i=0;i<1;i++)
{
	//mpz_mod(b,a,P256Para.n);
	Fast_mod256(b,a);
	//Barrett_modN(b,a);
}	
	mpz_printf(b);
	mpz_mod(b,a,P256Para.p);
	mpz_printf(b);
	finish = clock();
	double Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("%f seconds\n", Total_time);
}
void test_time()
{
	clock_t start, finish;
	start = clock();

	if(Cont.gmp_)
	{
		SignGen sign;
		test(&sign);
	}
	else if(Cont.openssl_)
	{
		EC_GROUP* group1;
		EC_KEY* key1;
		key1 = EC_KEY_new();
		group1 = EC_GROUP_new_by_curve_name(714);
		EC_KEY_set_group(key1, group1);
		EC_KEY_generate_key(key1);
		for (int i = 0; i < 500; i++)
		{
			//EC_KEY_generate_key(key1);
			teststd(key1);
		}
	}
	finish = clock();
	double Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("%f seconds\n", Total_time);
}
void testspy()
{
	mpz_t a;
	mpz_init_set_si(a,3);

	SPY Spy;

	EllPoint Point;
	Point.Setp(P256Para.x,P256Para.y);

	PMul p = &EllPoint::MulP;
	printf("%d\n",Spy.flush_reload((void *)p));
	Spy.flush((void *)p);
	printf("%d\n",Spy.flush_reload((void *)p));
}
int main(int argc,char** argv)
{
	
	Cont.InitControl(argc,argv);//test_time();
	//testspy();
	Spytest();
}



