#include"GMPecdsa.h"
#include<time.h>
#include<openssl/ecdsa.h>
#include<openssl/sha.h>

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
	
	//sign->key_pair_gen();
	sign->Ecdsa_sign_gen(Message);
	//sign->print();
	if (SignVerify::Ecdsa_sign_verify(&(sign->Q), Message, sign->r, sign->s))
	{
		//printf("验证成功\n");
	}
}
void test_time()
{
	clock_t start, finish;
	start = clock();

	if(Cont.gmp_)
	{
		SignGen sign;
		for (int i = 0; i < 500; i++)
		{
			test(&sign);
		}		
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



int main(int argc,char** argv)
{
	Cont.InitControl(argc,argv);
	test_time();
	//get_std_curve_name();
}



