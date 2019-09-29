#include <limits.h>
#include "bigint.h"

/* Atribuição (com extensão) */
void big_val (BigInt res, long val)
{
    int i;
    int sig = (val >> ((sizeof(val)*8)-1)) & 0x1; //pega o bit mais significativo de val
    unsigned char *walker = (unsigned char*)&val;
    for (i = 0;  i < (NUM_BITS/8); i++) {
        if (i < sizeof(val)) { //coloca o valor de val em res
            res[i] = *walker;
            walker++;
        } else if(sig) //se o numero for negativo completa os bits do vetor com 1
            res[i] = 0xff;
        else //se o numero for positivo completa os bits do vetor com 0
            res[i] = 0;
        
    }
}
/* Operações Aritméticas */

/* res = -a */
void big_comp2(BigInt res, BigInt a)
{
    int i, last, diff = 1;
    for (i=0; i < (NUM_BITS/8); i++) {
        res[i] = ~a[i];
        if(diff) {
        last = (res[i] >> 7) & 0x1;
        res[i]+=diff;
        diff = last ^ ((res[i] >> 7) & 0x1);
        }
    }
}

/* res = a + b */
void big_sum(BigInt res, BigInt a, BigInt b)
{
    int i, curr, old=0;
    for(i=0; i < (NUM_BITS/8); i++) {
        if ((a[i]+b[i] > UCHAR_MAX))
            curr = 1;
        res[i] = a[i]+b[i]+old;
        old = curr;
    }
}

/* res = a - b */
void big_sub(BigInt res, BigInt a, BigInt b)
{
    BigInt comp;
    big_comp2(comp, b); 
    big_sum(res, a, comp);
}

/* res = a * b */
void big_mul(BigInt res, BigInt a, BigInt b)
{

}

/* Operações de Deslocamento */

/* res = a << n */
void big_shl(BigInt res, BigInt a, int n)
{   
    unsigned char msb1;
    unsigned char msb2;

    for(int i = 0; i < NUM_BITS/8; i ++) //Setting up res
    {
        res[i] = a[i];
    }

    for (int i = 0; i < n; i++)
    {      
        msb1 = res[i/8] >> 7; //Getting the most significant bit

        res[i/8] = res[i/8] << 1;

        for (int j = i/8 + 1; j < NUM_BITS/8; j++) //Shifting the other elements 
        {   
            msb2 = res[j] >> 7; //Getting the most significant bit

            res[j] = res[j] << 1;

            res[j] |= msb1; //Setting the least significant bit to msb1

            msb1 = msb2;
        }
    }
    
}

/* res = a >> n (lógico)*/
void big_shr(BigInt res, BigInt a, int n)
{
    unsigned char lsb1;
    unsigned char lsb2;

    for(int i = 0; i < NUM_BITS/8; i ++) //Setting up res
    {
        res[i] = a[i];
    }

    for (int i = 0; i < n; i++)
    {
        lsb1 = 0; //Resetting lsb1 value

        for (int j = (NUM_BITS/8) - 1; j >= 0; j--)
        {   
            lsb2 = res[j] << 7; //Getting the least significant bit

            res[j] = res[j] >> 1;
 
            res[j] |= lsb1; //Setting the least significant bit to lsb1

            lsb1 = lsb2;
        }
    }
    
}

/* res = a >> n (aritmético)*/
void big_sar(BigInt res, BigInt a, int n)
{
    unsigned char aux = 0x80;

    big_shr(res, a, n);

    if((a[NUM_BITS/8 - 1] >> 7) & 0x01)
    {
        for(int i = 0; i < n; i++)
        {
            res[(NUM_BITS - i - 1)/8] |=  aux;
            aux >>= 1;

            if(!aux)
            {
                aux = 0x80;
            }
        }
    }
}