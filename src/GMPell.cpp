#include"GMPell.h"

FpEllPara P256Para;
FixedPoint FixPoint;
Control Cont;	

void Fast_mod256(mpz_t rop, mpz_t op)
{
	unsigned long long int myop[16]={0};
	for(int i=0;i< op->_mp_size;i++)
	{
		myop[2*i] = (unsigned int)op->_mp_d[i];
		myop[2*i+1] = (unsigned int)(op->_mp_d[i]>>32);
	}
	unsigned long long int mid=0;
	unsigned int c[9];

	mid = myop[0]+myop[8]+myop[9];
	c[0]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[1]+myop[9]+myop[10]+mid;
	c[1]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[2]+myop[10]+myop[11]+mid;
	c[2]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[3]+2*myop[11]+2*myop[12]+myop[13]+mid;
	c[3]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[4]+2*myop[12]+2*myop[13]+myop[14]+mid;
	c[4]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[5]+2*myop[13]+2*myop[14]+myop[15]+mid;
	c[5]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[6]+3*myop[14]+2*myop[15]+myop[13]+mid;
	c[6]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[7]+3*myop[15]+myop[8]+mid;
	c[7]=(unsigned int)mid;
	mid = mid>>32;

	c[8]=mid;

	unsigned int d[9];
	mid = myop[11]+myop[12]+myop[13]+myop[14];
	d[0]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[15]+myop[12]+myop[13]+myop[14]+mid;
	d[1]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[15]+myop[13]+myop[14]+mid;
	d[2]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[15]+myop[8]+myop[9]+mid;
	d[3]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[9]+myop[10]+mid;
	d[4]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[11]+myop[10]+mid;
	d[5]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[8]+myop[9]+mid;
	d[6]=(unsigned int)mid;
	mid = mid>>32;

	mid = myop[11]+myop[12]+myop[13]+myop[10]+mid;
	d[7]=(unsigned int)mid;
	mid = mid>>32;

	d[8]=mid;

	mpz_t mpzc,mpzd;
	mpz_init2(mpzc,512);
	mpz_init2(mpzd,512);

	mpz_import(mpzc,9,-1,4,0,0,c);
	mpz_import(mpzd,9,-1,4,0,0,d);
	mpz_sub(mpzc,mpzc,mpzd);
	
	while(mpz_cmp(mpzc,P256Para.p)>=0)
	{
		mpz_sub(mpzc,mpzc,P256Para.p);
	}
	while(mpz_cmp_si(mpzc,0)<0)
	{
		mpz_add(mpzc,mpzc,P256Para.p);
	}
	mpz_set(rop,mpzc);
	mpz_clear(mpzc);
	mpz_clear(mpzd);
}
void Barrett_modN(mpz_t rop,mpz_t op)
{
	mpz_t q,r;
	mpz_init(q);
	mpz_init(r);

	mpz_fdiv_q_2exp(q,op,255);
	mpz_mul(q,q,P256Para.u);
	mpz_fdiv_q_2exp(q,q,257);

	mpz_fdiv_r_2exp(r,op,257);
	mpz_mul(q,q,P256Para.n);
	mpz_fdiv_r_2exp(q,q,257);
	mpz_sub(r,r,q);
	if(mpz_cmp_si(r,0)<0)
	{
		mpz_t b;
		mpz_init_set_si(b,1);
		mpz_mul_2exp(b,b,257);
		mpz_add(r,r,b);
		mpz_clear(b);
	}
	while(mpz_cmp(r,P256Para.n)>=0)
	{
		mpz_sub(r,r,P256Para.n);
	}
	mpz_set(rop,r);
	mpz_clear(q);
	mpz_clear(r);
}
void mpz_printf(mpz_t op)
{
	for(int i=op->_mp_size-1;i>=0;i--)
	{
		printf("%lx",op->_mp_d[i]);
	}
	printf("\n");
}
void Fp_mod(mpz_t rop,mpz_t op,int modnum)
{
	if(modnum==ModP)
	{
		if(Cont.Fast_red)
		{
			Fast_mod256(rop,rop);
		}	
		else{
			mpz_mod(rop, rop, P256Para.p);
		}
	}
	else
	{
		if(Cont.Burr_red)
		{
			Barrett_modN(rop,rop);
		}
		else{
			mpz_mod(rop,rop,P256Para.n);
		}
	}
}

inline void Fp_Mul(mpz_t rop, mpz_t op1, mpz_t op2)
{
	mpz_mul(rop, op1, op2);
	if(Cont.Fast_red)
	{
		Fast_mod256(rop,rop);
	}	
	else
	{
		mpz_mod(rop, rop, P256Para.p);
	}
}
void Fp_MulN(mpz_t rop, mpz_t op1, mpz_t op2)
{
	mpz_mul(rop, op1, op2);
	if(Cont.Burr_red)
	{
		Barrett_modN(rop,rop);
	}
	else{
		mpz_mod(rop,rop,P256Para.n);
	}
}
inline void Fp_Add(mpz_t rop, mpz_t op1, mpz_t op2)
{
	mpz_add(rop, op1, op2);
	//Fp_mod(rop,rop,modnum);
	if(mpz_cmp(rop,P256Para.p)>=0)
	{
		mpz_sub(rop,rop,P256Para.p);
	}
}
inline void Fp_Sub(mpz_t rop, mpz_t op1, mpz_t op2)
{
	mpz_sub(rop, op1, op2);
	if(mpz_cmp_si(rop,0)<0)
	{
		mpz_add(rop,rop,P256Para.p);
	}
}
void Mpz2wNAF(mpz_t x, vector<long int>& wNaf, int w)
{
	mpz_t k;
	mpz_init_set(k, x);
	int i = 0;
	unsigned long int d = 1 << w;
	unsigned long int m = d / 2;
	vector<long int> mi;
	while (mpz_cmp_si(k, 1) >= 0)
	{
		if (mpz_tstbit(k, 0))
		{
			int j = mpz_fdiv_ui(k, d);
			j = j > m ? j - d : j;
			mi.push_back(j);
			if (j < 0)
			{
				mpz_add_ui(k, k, 0 - j);
			}
			else
			{
				mpz_sub_ui(k, k, j);
			}
		}
		else
		{
			mi.push_back(0);
		}
		mpz_divexact_ui(k, k, 2);
		i++;
	}
	int n = mi.size();
	for (int i = 0; i < n; i++)
	{
		wNaf.push_back(mi[n - i - 1]);
	}
	mpz_clear(k);
}
void Pointaddxy(EllPoint* p){
	if(p->Is_inf())
	{
		mpz_set_si(p->x,0);
		mpz_set_si(p->y,0);
	}
	else{
		mpz_t mid;
		mpz_init(mid);
		mpz_t z_1;
		mpz_init(z_1);
		mpz_invert(z_1, p->jz,P256Para.p);
		Fp_Mul(mid,z_1,z_1);
		Fp_Mul(p->x, p->jx, mid);

		Fp_Mul(mid, mid,z_1);
		Fp_Mul(p->y,p->jy, mid);
		
		mpz_clear(mid);
		mpz_clear(z_1);
	}
	p->Is_xy = 1;
}
EllPoint::EllPoint()
{
	mpz_init_set_si(x,0);
	mpz_init_set_si(y, 0);
	mpz_init_set_si(jx,0);
	mpz_init_set_si(jy,1);
	mpz_init_set_si(jz, 0);
	Is_xy =1;
}
EllPoint::EllPoint(mpz_t x, mpz_t y)
{
	long int zero= 0;
	if (mpz_cmp_si(x, zero) == 0 && mpz_cmp_si(y, zero)==0)
	{
		mpz_init_set_si(this->x, 0);
		mpz_init_set_si(this->y, 0);
		mpz_init_set_si(jx, 0);
		mpz_init_set_si(jy, 1);
		mpz_init_set_si(jz, 0);
	}
	else
	{
		mpz_init_set(this->x, x);
		mpz_init_set(this->y, y);
		mpz_init_set(jx, x);
		mpz_init_set(jy, y);
		mpz_init_set_si(jz, 1);
	}
	Is_xy = 1;
}
EllPoint::EllPoint(mpz_t x, mpz_t y, mpz_t z)
{
	long int zero = 0;
	long int one = 1;
	if (mpz_cmp_si(x, zero) == 0 && mpz_cmp_si(y, one) == 0 && mpz_cmp_si(z, zero) == 0)
	{
		mpz_init_set_si(this->x, 0);
		mpz_init_set_si(this->y, 0);
		mpz_init_set_si(jx, 0);
		mpz_init_set_si(jy, 1);
		mpz_init_set_si(jz, 0);
		Is_xy=1;
	}
	else
	{
		mpz_init_set(jx, x);
		mpz_init_set(jy, y);
		mpz_init_set(jz, z);
		mpz_init(x);
		mpz_init(y);
		Is_xy = 0;
	}
	
}
EllPoint::~EllPoint()
{
	mpz_clear(x);
	mpz_clear(y);
	mpz_clear(jx);
	mpz_clear(jy);
	mpz_clear(jz);
}
void EllPoint::Setp(mpz_t x, mpz_t y)
{
	if(mpz_cmp_si(x, 0) == 0 && mpz_cmp_si(y, 0)==0){
		mpz_set_si(this->x, 0);
		mpz_set_si(this->y, 0);
		mpz_set_si(jx, 0);
		mpz_set_si(jy, 1);
		mpz_set_si(jz, 0);
	}
	else{
		mpz_set(this->x, x);
		mpz_set(this->y, y);
		mpz_set(jx, x);
		mpz_set(jy, y);
		mpz_set_si(jz, 1);
	}
	Is_xy =1;
 }
void EllPoint::Setp(mpz_t x, mpz_t y, mpz_t z)
{
	mpz_set(jx, x);
	mpz_set(jy, y);
	mpz_set(jz, z);
	Is_xy =0;
}
void EllPoint::Setp(EllPoint* op)
{
	if(op->Is_xy==1)
	{
		Setp(op->x, op->y);
	}
	else{
		Setp(op->jx, op->jy,op->jz);
	}
	
}
bool EllPoint::Is_inf()
{
	long int zero = 0;
	if (mpz_cmp_si(jx, zero) == 0 && mpz_cmp_si(jy, 1)==0&&mpz_cmp_si(jz, 0)==0)
	{
		return true;
	}
	return false;
}
bool EllPoint::Is_in_ell()
{
	if (Is_inf())
	{
		return true;
	}
	if(Is_xy==0)
	{
		Pointaddxy(this);
	}
	mpz_t y2, x3, ax;
	mpz_init(y2);
	mpz_init(x3);
	mpz_init(ax);
	Fp_Mul(y2, y, y);
	Fp_Mul(x3, x,  x);
	Fp_Mul(x3, x3,x);
	Fp_Mul(ax, P256Para.a, x);
	Fp_Add(x3, x3, ax);
	Fp_Add(x3, x3, P256Para.b);
	if (mpz_cmp(y2, x3) == 0)
	{
		mpz_clear(y2);
		mpz_clear(x3);
		mpz_clear(ax);
		return true;
	}
	mpz_clear(y2);
	mpz_clear(x3);
	mpz_clear(ax);
	return false;
}
void EllPoint::print()
{
	if(Is_xy==0)
	{
		Pointaddxy(this);
	}
	printf("x:");
	mpz_printf(x);
	printf("y:");
	mpz_printf(y);
	printf("\n");
}

void EllPoint::Pdouble(EllPoint* rop)
{
	if (rop->Is_inf())
	{
		Setp(rop);
		return;
	}
	mpz_t t1,t2,t3,t4;
	mpz_t x3, y3, z3;
	mpz_init(t1);
	mpz_init(t2);
	mpz_init(t3);
	mpz_init(t4);
	mpz_init(x3);
	mpz_init(y3); 
	mpz_init(z3);

	Fp_Mul(t1, rop->jz, rop->jz);
	Fp_Sub(t2, rop->jx, t1);
	Fp_Add(t1, rop->jx, t1);
	Fp_Mul(t2, t2, t1);
	Fp_Add(t4, t2, t2);
	Fp_Add(t2, t2, t4);
	Fp_Add(y3, rop->jy, rop->jy);
	Fp_Mul(z3, y3, rop->jz);
	Fp_Mul(y3, y3, y3);
	Fp_Mul(t3, y3, rop->jx);
	Fp_Mul(y3, y3, y3);
	if (mpz_divisible_2exp_p(y3, 1) != 0)
	{
		mpz_divexact_ui(y3, y3, 2);
	}
	else
	{
		mpz_add(y3, y3, P256Para.p);
		mpz_divexact_ui(y3, y3, 2);
	}
	Fp_Mul(x3, t2, t2);
	Fp_Add(t1, t3, t3);
	Fp_Sub(x3, x3, t1);
	Fp_Sub(t1, t3, x3);
	Fp_Mul(t1, t1, t2);
	Fp_Sub(y3, t1, y3);

	Setp(x3, y3, z3);
	mpz_clear(t1);
	mpz_clear(t2);
	mpz_clear(t3);
	mpz_clear(t4);
	mpz_clear(x3);
	mpz_clear(y3);
	mpz_clear(z3);
}
void EllPoint::Add(EllPoint *p, EllPoint *q)
{
	if (p->Is_inf())
	{
		Setp(q);
		return;
	}
	if(q->Is_inf())
	{
		Setp(p);
		return;
	}
	if(q->Is_xy==0)
	{
		Pointaddxy(q);
	}
	mpz_t t1,t2,t3,t4;
	mpz_t x3, y3, z3;
	mpz_init(t1);
	mpz_init(t2);
	mpz_init(t3);
	mpz_init(t4);
	mpz_init(x3);
	mpz_init(y3);
	mpz_init(z3);

	Fp_Mul(t1, p->jz, p->jz);
	Fp_Mul(t2, t1, p->jz);
	Fp_Mul(t1, t1, q->x);
	Fp_Mul(t2, t2, q->y);
	Fp_Sub(t1, t1, p->jx);
	Fp_Sub(t2, t2, p->jy);
	long int zero = 0;
	if (mpz_cmp_si(t1, zero) == 0 )
	{
		if (mpz_cmp_si(t2, zero) == 0)
		{
			Pdouble(q);
			mpz_clear(t1);
			mpz_clear(t2);
			mpz_clear(t3);
			mpz_clear(t4);
			mpz_clear(x3);
			mpz_clear(y3);
			mpz_clear(z3);
			return;
		}
		else
		{
			mpz_set_si(x, 0);
			mpz_set_si(y, 0);
			mpz_set_si(jx, 0);
			mpz_set_si(jy, 1);
			mpz_set_si(jz, 0);
			mpz_clear(t1);
			mpz_clear(t2);
			mpz_clear(t3);
			mpz_clear(t4);
			mpz_clear(x3);
			mpz_clear(y3);
			mpz_clear(z3);
			return;
		}
	}
	Fp_Mul(z3, p->jz, t1);
	Fp_Mul(t3, t1, t1);
	Fp_Mul(t4, t3, t1);
	Fp_Mul(t3, t3, p->jx);
	Fp_Add(t1, t3, t3);
	Fp_Mul(x3, t2, t2);
	Fp_Sub(x3, x3, t1);
	Fp_Sub(x3,x3, t4);
	Fp_Sub(t3, t3, x3);
	Fp_Mul(t3, t3, t2);
	Fp_Mul(t4, t4, p->jy);
	Fp_Sub(y3, t3, t4);

	Setp(x3,y3,z3);

	mpz_clear(t1);
	mpz_clear(t2);
	mpz_clear(t3);
	mpz_clear(t4);
	mpz_clear(x3);
	mpz_clear(y3);
	mpz_clear(z3);
}
void EllPoint::Sub(EllPoint* p, EllPoint* q)
{
	EllPoint mi;
	unsigned long int zero = 0;
	mpz_t y;
	mpz_init_set_ui(y,0);
	if(q->Is_xy==0)
	{
		Pointaddxy(q);
	}
	Fp_Sub(y, y, q->y);
	mi.Setp(q->x, y);
	Add(p, &mi);
	mpz_clear(y);
}


void EllPoint::wNafMul(mpz_t k, EllPoint* op)
{
	vector<long int> Naf;
	Mpz2wNAF(k, Naf, NAF_WIN_LEN);
	int d = 1 << NAF_WIN_LEN;
	EllPoint PI[1 << (NAF_WIN_LEN - 2)];
	EllPoint DBop;
	EllPoint inf;
	PI[0].Setp(op);
	DBop.Pdouble(op);
	for (int i = 1; i < d / 4; i++)
	{
		PI[i].Add(&PI[i - 1], &DBop);
	}
	int l = Naf.size();
	for (int i = 0; i < l; i++)
	{
		inf.Pdouble(&inf);
		if (Naf[i] != 0)
		{
			if (Naf[i] > 0)
			{
				inf.Add(&inf, &PI[(Naf[i] - 1) / 2]);
			}
			else
			{
				inf.Sub(&inf, &PI[(0 - Naf[i] - 1) / 2]);
			}
		}

	}
	Setp(&inf);
	Pointaddxy(this);
}
void EllPoint::FixedMul(mpz_t k)
{
	vector<long int> Naf;
	Mpz2wNAF(k, Naf,2);
	EllPoint inf;
	int l = Naf.size();
	for (int i = l - 1; i >= 0; i--)
	{
		if (Naf[i] == 1)
		{
			inf.Add(&inf, FixPoint.FixedP[l - i - 1]);
		}
		else if (Naf[i] == -1)
		{
			inf.Sub(&inf, FixPoint.FixedP[l - i - 1]);
		}

	}
	Setp(&inf);
	Pointaddxy(this);
}
void EllPoint::Mul(mpz_t k, EllPoint* op)
{
	if(Cont.wNaf)
	{
		wNafMul(k,op);
		return;
	}
	if (op->Is_inf())
	{
		Setp(op);
		return;
	}
	EllPoint inf;
	for (int i = mpz_sizeinbase(k, 2) - 1; i >= 0; i--)
	{
		inf.Pdouble(&inf);
		if (mpz_tstbit(k, i))
		{
			inf.Add(&inf, op);
		}
	}
	Setp(&inf);
	Pointaddxy(this);
}
void EllPoint::MulP(mpz_t k,EllPoint* op)
{
	if (Cont.Fixed_base_)
	{
		FixedMul(k);
		return;
	}
	Mul(k,op);
	Pointaddxy(this);
}
void EllPoint::MP_Mul(mpz_t k,mpz_t l,EllPoint *P,EllPoint* Q)
{
	EllPoint R[1<<MultiWin][1<<MultiWin];
	EllPoint inf;
	R[1][0].Setp(FixPoint.FixedP[0]);
	R[2][0].Setp(FixPoint.FixedP[1]);
	R[3][0].Add(&R[1][0],&R[2][0]);
	R[0][1].Setp(Q);
	for(int j=1;j<3;j++)
	{
		R[0][j+1].Add(&R[0][j],Q);
	}
	for(int j=1;j<4;j++)
	{
		for(int i=1;i<4;i++)
		{
			R[i][j].Add(&R[i-1][j],P);
		}	
	}
	int i = mpz_sizeinbase(k, 2)-1;
	int j = mpz_sizeinbase(l, 2) -1;
	i = i>j?i:j;
	i+=(i+1)%2;
	int m,n;

	for (; i >= 0; i-=2)
	{
		inf.Pdouble(&inf);
		inf.Pdouble(&inf);
		m = (mpz_tstbit(k,i)<<1) + mpz_tstbit(k,i-1);
		n= (mpz_tstbit(l,i)<<1) + mpz_tstbit(l,i-1);
		inf.Add(&R[m][n],&inf);
	}
	Setp(&inf);
	Pointaddxy(this);
}

