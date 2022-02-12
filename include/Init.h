#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<gmp.h>
#include<vector>
#include<time.h>
#include<openssl/sha.h>
#include<openssl/ecdsa.h>
using namespace std;
#define Para_ 256//192,224,256,384,521
#define NAF_WIN_LEN 6//>=2

#if Para_ == 192
#define ParaSize_ 160
#define Fixedlen 195
#define BASE_P "ffffffff ffffffff ffffffff fffffffe ffffffff ffffffff"                     
#define BASE_X "188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012"
#define BASE_Y "07192b95 ffc8da78 631011ed 6b24cdd5 73f977a1 1e794811"
#define PARA_A "fffffffffffffffffffffffffffffffefffffffffffffffc"  
#define PARA_B "64210519 e59c80e7 0fa7e9ab 72243049 feb8deec c146b9b1"
#define PARA_R "3099D2bb bfcb2538 542dcd5f b078b6ef 5f3d6fe2 c745de65"
#define PARA_S "3045ae6f c8422f64 ed579528 d38120ea e12196d5"
#define PARA_N "ffffffff ffffffff ffffffff 99def836 146bc9b1 b4d22831"
#elif Para_ == 224
#define ParaSize_ 224
#define Fixedlen 230
#define BASE_P "ffffffffffffffffffffffffffffffff000000000000000000000001"                     
#define BASE_X "B70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6115C1D21"
#define BASE_Y "BD376388B5F723FB4C22DFE6CD4375A05A07476444D5819985007E34"
#define PARA_A "ffffffff ffffffff ffffffff fffffffe ffffffff ffffffff fffffffe"
#define PARA_B "B4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4"
#define PARA_R "5B056C7E11DD68F40469EE7F3C7A7D74F7D121116506D031218291FB"
#define PARA_S "BD71344799D5C7FCDC45B59FA3B9AB8F6A948BC5"
#define PARA_N "FFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D"
#elif Para_ == 256
#define ParaSize_ 256
#define Fixedlen 260
#define BASE_P "ffffffff00000001000000000000000000000000ffffffffffffffffffffffff"                     
#define BASE_X "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296"
#define BASE_Y "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5"
#define PARA_A "ffffffff00000001000000000000000000000000fffffffffffffffffffffffc"
#define PARA_B "5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b"   
#define PARA_R "7efba1662985be9403cb055c75d4f7e0ce8d84a9c5114abcaf3177680104fa0d" 
#define PARA_S "c49d360886e704936a6678e1139d26b7819f7e90" 
#define PARA_N "ffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551"
#define PARA_U "1 00000000 ffffffff fffffffe ffffffff 43190552 df1a6c21 012ffd85 eedf9bfe"
#elif Para_ == 384
#define ParaSize_ 384
#define Fixedlen 390
#define BASE_P "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff0000000000000000ffffffff"                     
#define BASE_X "aa87ca22be8b05378eb1c71ef320ad746e1d3b628ba79b9859f741e082542a385502f25dbf55296c3a545e3872760ab7"
#define BASE_Y "3617de4a96262c6f5d9e98bf9292dc29f8f41dbd289a147ce9da3113b5f0b8c00a60b1ce1d7e819d7a431d7c90ea0e5f"
#define PARA_A "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff0000000000000000fffffffc"
#define PARA_B "b3312fa7e23ee7e4988e056be3f82d19181d9c6efe8141120314088f5013875ac656398d8a2ed19d2a85c8edd3ec2aef"
#define PARA_R "79d1e655f868f02fff48dcdee14151ddb80643c1406d0ca10dfe6fc52009540a495e8042ea5f744f6e184667cc722483"
#define PARA_S "a335926aa319a27a1d00896a6773a4827acdac73"
#define PARA_N "ffffffffffffffffffffffffffffffffffffffffffffffffc7634d81f4372ddf581a0db248b0a77aecec196accc52973"
#elif Para_ == 521
#define ParaSize_ 512
#define Fixedlen 525
#define BASE_P "1fffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff fff"                     
#define BASE_X "000000C6 858E06B7 0404E9CD 9E3ECB66 2395B442 9C648139 053FB521 F828AF60 6B4D3DBA A14B5E77 EFE75928 FE1DC127 A2FFA8DE 3348B3C1 856A429B F97E7E31 C2E5BD66"
#define BASE_Y "00000118 39296A78 9A3BC004 5C8A5FB4 2C7D1BD9 98F54449 579B4468 17AFBD17 273E662C 97EE7299 5EF42640 C550B901 3FAD0761 353C7086 A272C240 88BE9476 9FD16650"
#define PARA_A "1fffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffc"
#define PARA_B "00000051 953EB961 8E1C9A1F 929A21A0 B68540EE A2DA725B 99B315f3 b8b48991 8EF109E1 56193951 EC7E937B 1652C0BD 3BB1BF07 3573df88 3D2C34F1 EF451FD4 6B503F00"
#define PARA_R "000000B4 8BFA5F42 0A349495 39D2BDFC 264EEEEB 077688E4 4FBF0AD8 F6D0EDB3 7BD6B533 28100051 8E19F1B9 FFBE0FE9 ED8A3C22 00B8F875 E523868C 70C1E5BF 55BAD637"
#define PARA_S "D09E8800 291CB853 96CC6717 393284AA A0DA64BA"
#define PARA_N "000001FF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFA 51868783 BF2F966B 7FCC0148 F709A5D0 3BB5C9B8 899C47AE BB6FB71E 91386409"
#endif//Para

class FpEllPara
{
private:

public:
	FpEllPara()
	{
		mpz_init_set_str(p, BASE_P, 16);
		mpz_init_set_str(x, BASE_X, 16);
		mpz_init_set_str(y, BASE_Y, 16);
		mpz_init_set_str(a, PARA_A, 16);
		mpz_init_set_str(b, PARA_B, 16);
		mpz_init_set_str(r, PARA_R, 16);
		mpz_init_set_str(S, PARA_S, 16);
		mpz_init_set_str(n, PARA_N, 16);
		mpz_init_set_str(u, PARA_U, 16);
	}
	~FpEllPara()
	{
		mpz_clear(p);
		mpz_clear(x);
		mpz_clear(y);
		mpz_clear(a);
		mpz_clear(b);
		mpz_clear(r);
		mpz_clear(S);
		mpz_clear(n);
		mpz_clear(u);
	}
	mpz_t p;
	mpz_t x;
	mpz_t y;
	mpz_t a;
	mpz_t b;
	mpz_t r;
	mpz_t S;
	mpz_t n;
	mpz_t u;
};
extern FpEllPara P256Para;

class Control
{
public:
	bool gmp_;
	bool openssl_;
	bool wNaf;
	bool Fixed_base_;
	bool Burr_red;
	bool Fast_red;
	int size;
	Control()
	{
		gmp_ = 1;
		openssl_=0;
		wNaf =1;
		Fixed_base_=1;
		Burr_red=1;
		Fast_red=1;
	}
	~Control()
	{

	}
	void InitControl(int argc,char** argv)
	{
		switch (argc)
		{
		case 1:
			break;
		case 2:
			if(strcmp((const char*)argv[1],"GMP")==0||strcmp((const char*)argv[1],"gmp")==0){break;}
			else if(strcmp((const char*)argv[1],"OPENSSL")==0||strcmp((const char*)argv[1],"openssl")==0)
			{
				gmp_=0;
				openssl_=1;
				break;
			}
		default:
			for(int i=1;i<argc;i++)
			{
				if(strcmp((const char*)argv[i],"wNAF")==0)
				{
					wNaf=0;
				}
				else if(strcmp((const char*)argv[i],"FixedBase")==0)
				{
					Fixed_base_=0;
				}
				else if(strcmp((const char*)argv[i],"BurrMod")==0)
				{
					Burr_red=0;
				}
				else if(strcmp((const char*)argv[i],"FastMod")==0)
				{
					Fast_red=0;
				}
			}
			break;
		}
	}
};
extern Control Cont;	
