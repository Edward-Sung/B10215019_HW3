#include<iostream>
#include<gmpxx.h>
using namespace std;

bool miller_rabin(mpz_t p)
{
    gmp_randstate_t s;
    gmp_randinit_mt (s);
    mpz_t a,u,t,f,x,z;
    mpz_init(t);
    mpz_init(a);
    mpz_init(u);
    mpz_init(f);
    mpz_init(x);
    mpz_init(z);

    mpz_urandomb (a,s,512);
    mpz_mod(a,a,p);
    mpz_sub_ui(u,p,1);
    mpz_mod_ui(f,u,2);

    while (mpz_cmp_ui(f,0) == 0)
    {
        mpz_divexact_ui(u,u,2);
        mpz_add_ui(t,t,1);
        mpz_mod_ui(f,u,2);
    }

    mpz_powm(x,a,u,p);

    mpz_sub_ui(z,p,1);

    if (mpz_cmp_ui(x,1) == 0 || mpz_cmp(x,z) == 0) return true;

    mpz_sub_ui(t,t,1);

    for (int i=0; mpz_cmp_ui(t,i)>0; i++)
    {
        mpz_mul(x,x,x);
        mpz_mod(x,x,p);
        if (mpz_cmp_ui(x,1)==0 ) return false;
        if (mpz_cmp(x,z)== 0) return true;
    }
    return false;
}

void power(mpz_t y,mpz_t x,mpz_t h,mpz_t n){
    char* c = mpz_get_str(NULL,2,h);
    mpz_set(y,x);

    for(int i=1;c[i]!='\0';i++){
        mpz_pow_ui(y,y,2);
        mpz_mod(y,y,n);
        if(c[i]=='1'){
            mpz_mul(y,y,x);
            mpz_mod(y,y,n);
        }
    }

}

void CRT(mpz_t a,mpz_t c,mpz_t p,mpz_t q,mpz_t d){
    mpz_t m1,m2,t,qi;
    mpz_init(m1);
    mpz_init(m2);
    mpz_init(t);
    mpz_init(qi);

    mpz_invert(qi,q,p);



    mpz_sub_ui(t,p,1);
    mpz_mod(t,d,t);
    mpz_powm(m1,c,t,p);

    mpz_sub_ui(t,q,1);
    mpz_mod(t,d,t);
    mpz_powm(m2,c,t,q);

    mpz_sub(t,m1,m2);
    mpz_mul(t,qi,t);
    mpz_mod(t,t,p);
    mpz_mul(t,t,q);
    mpz_add(a,m2,t);


}


int main()
{
    mpz_t n,p,q,c,e,r,d,y,x,t1,t2;
    int i=0,l;
    gmp_randstate_t s;
    mpz_init(n);
    mpz_init(p);
    mpz_init(q);
    mpz_init(c);
    mpz_init(e);
    mpz_init(r);
    mpz_init(d);
    mpz_init(y);
    mpz_init(x);
    mpz_init(t1);
    mpz_init(t2);
    gmp_randinit_mt(s);

    cout << "Choose key size(1024/2048/4096): " ;
    cin >> l ;


    mpz_set_ui(p,3);
    mpz_set_ui(q,11);

    mpz_urandomb (p, s,l/2);


    mpz_urandomb (q, s,l/2);



    while(!miller_rabin(p))          //prime test
        mpz_urandomb (p, s,l/2);

    while(!miller_rabin(q))          //prime test
        mpz_urandomb (q, s,l/2);

    cout << "p = " << p << endl ;
    cout << "q = " << q << endl ;

    mpz_mul(n,p,q);  // n=p*q

    mpz_sub_ui(t1,p,1);
    mpz_sub_ui(t2,q,1);
    mpz_mul(c,t1,t2); //  c = (p-1)(q-1)

    for(i=2; mpz_cmp_ui(c,i)>0; i++)    // find e
    {
        mpz_mod_ui(r,c,i);
        if(mpz_cmp_ui(r,0)!=0)
        {
            mpz_set_ui(e,i);
            break;
        }
    }



    mpz_invert(d,e,c);          //find d

    cout << "Input message: ";
    cin >> x;

    power(y,x,e,n);

    cout << "Message after encrypt: " << y << endl;

    cout << "Choose method to Decrypt(1:square/2:CRT): ";

    cin >> l;

    if (l==1)
        power(x,y,d,n);
    else
        CRT(x,y,p,q,d);


    cout << "Message after decrypt: " << x;



    return 0;
}
