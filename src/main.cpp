#include"GMPecdsa.h"
#include"spy.h"
#include"cmdline.h"
#define  message "information security"
#pragma GCC diagnostic ignored "-Wpmf-conversions"
#pragma GCC diagnostic ignored "-Wattributes"
char Message[1024];// = message;
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
void help_info()
{
	printf("\nhelp   print help info\n");
	printf("sign   generate the signature\n");
	printf("verify  verify the signature\n");
	printf("key    generate the key\n");
	printf("exit    exit\n");
}
void interact()
{
	char str[500];
	help_info();
	SignGen sign;
	while(true){
		scanf("%s",str);
		if(strcmp(str,"help")==0)
		{
			help_info();
		}
		else if(strcmp(str,"sign")==0)
		{
			sign.Ecdsa_sign_gen(Message);
			sign.print();
			printf("\n");
		}
		else if(strcmp(str,"verify")==0){
			if(SignVerify::Ecdsa_sign_verify(&(sign.Q), Message, sign.r, sign.s)){
				printf("verify success\n\n");
			}
		}
		else if(strcmp(str,"key")==0){
			sign.key_pair_gen();
			printf("Private key:");
			mpz_printf(sign.d);
			printf("Public key:\n");
			sign.Q.print();
		}
		else if(strcmp(str,"exit")==0)
		{
			printf("app exit\n");
			break;
		}
	}
}
void test_openssl(bool ver)
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
			if (ver){
				ECDSA_do_verify(buf, 32, s, key1);
			}
		}
		finish = clock();
		double Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
		printf("%f seconds\n", Total_time);
}
void test_security_encrypt(bool ver)
{
	clock_t start, finish;
	start = clock();

	Safe_sign sign;
	for (int i = 0; i < number; i++)
	{
		sign.Ecdsa_sign_gen(Message);
		if(ver){
			SignVerify::Ecdsa_sign_verify(&(sign.Q), Message, sign.r, sign.s);
		}
	}	
	finish = clock();
	double Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("%f seconds\n", Total_time);
}
void test_spy()
{
	//testspy();
	Spytest();
	printf("spy\n");
}
void test_my_encrypt(bool ver)
{
	clock_t start, finish;
	start = clock();
	SignGen sign;
	for (int i = 0; i < number; i++)
	{
		sign.Ecdsa_sign_gen(Message);
		if(ver)
		{
			SignVerify::Ecdsa_sign_verify(&(sign.Q), Message, sign.r, sign.s);
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
	a.add<string>("Message",'\0',"The Signed Message",false,"information security");
	
	a.add("openssl",'o',"use openssl");

	a.add("spy",'\0',"cache side channel");

	a.add("security",'s',"Montgomery Ladder");

	a.add("wNAF",'w',"window NAF");
	a.add("fixed",'f',"Fixed-base");
	a.add("Bar",'b',"Barrett reduction");
	a.add("Fast",'F',"Fast reduction");
	a.add("MultiPoint",'m',"Multiple Point Multiplication");
	a.add("interact",'i',"interact with app");
	a.add("verify",'v',"with verification");
	a.parse_check(argc,argv);
	number = a.get<int>("number");
	strcpy(Message,a.get<string>("Message").c_str());

	
	Cont.wNaf = a.exist("wNAF");
	Cont.Fixed_base_ = a.exist("fixed");
	Cont.Burr_red = a.exist("Bar");
	Cont.Fast_red = a.exist("Fast");
	Cont.Muti = a.exist("MultiPoint");

	if(a.exist("openssl"))
	{
		test_openssl(a.exist("verify"));
	}
	else if(a.exist("spy")){
		test_spy();
	}
	else if(a.exist("security")){	
		test_security_encrypt(a.exist("verify"));
	}
	else if(a.exist("interact")){
		interact();
	}
	else{
		test_my_encrypt(a.exist("verify"));
	}
}
int main(int argc,char** argv)
{
	test1(argc,argv);

	//testspy();
}



