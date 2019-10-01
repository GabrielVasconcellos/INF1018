/* Bernardo Rezende 1811441 3WA */
/* Gabriel Luiz Vasconcellos 1810542 3WA */
#include "bigint.h"

/* Funções Auxiliares */

/* res = 0 ? */
int big_isZero(BigInt a) 
{
    int i;

    for (i=0; i < (NUM_BITS/8) - 1; i++)
    {
        if (a[i] != 0)
            return 0;
    }
    return 1;
}

/* res = a */
void big_copy(BigInt res, BigInt a) {
    int i;

    for (i=0; i < NUM_BITS/8; i++)
        res[i] = a[i];
}

/* Atribuição (com extensão) */
void big_val (BigInt res, long val)
{
    int i;
    int sig = (val >> ((sizeof(val)*8)-1)) & 0x1; //pega o bit mais significativo de val
    unsigned char *walker = (unsigned char*) & val;

    for (i = 0;  i < (NUM_BITS/8); i++)
    {
        if (i < sizeof(val)) //coloca o valor de val em res
        { 
            res[i] = *walker;
            walker++;
        }
        else if(sig) //se o numero for negativo completa os bits do vetor com 1
            res[i] = 0xff;
        else //se o numero for positivo completa os bits do vetor com 0
            res[i] = 0;
    }
}

/* Operações Aritméticas */

/* res = -a */
void big_comp2(BigInt res, BigInt a)
{
    int i, diff = 1;

    for (i=0; i < (NUM_BITS/8); i++) {
        res[i] = ~a[i];
        if(diff) {
            diff += res[i];
            res[i] = diff & 0xff;
            diff >>= 8;
        }
    }
}

/* res = a + b */
void big_sum(BigInt res, BigInt a, BigInt b)
{
    unsigned int curr=0;

    for(int i = 0; i < (NUM_BITS/8); i++)
    {
        curr += a[i]+b[i];
        res[i] = curr & 0xff;
        curr >>= 8;
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
    BigInt funcA;
    BigInt funcB;
    int sig = 0;

    big_copy(funcA, a);
    big_copy(funcB, b);
    big_val(res, 0);  
    
    if ((funcA[(NUM_BITS/8)-1] >> 7) & 0x1)
    {
        sig = 1;
        big_comp2(funcA, funcA);
    }
    if ((funcB[(NUM_BITS/8)-1] >> 7) & 0x1)
    {
        sig ^= 1;
        big_comp2(funcB, funcB);
    } 
    while (!big_isZero(funcB)) 
    { 
        if (funcB[0] & 0x1) 
            big_sum(res, res, funcA); 
        
        big_shl(funcA, funcA, 1); 
        big_shr(funcB, funcB, 1);
    }
    if(sig)
        big_comp2(res, res); 
}   

/* Operações de Deslocamento */

/* res = a << n */
void big_shl(BigInt res, BigInt a, int n)
{   
    unsigned char msb1;
    unsigned char msb2;

    big_copy(res, a);
    if (n >= NUM_BITS)
        return;

    for (int i = 0; i < n; i++)
    {      
        msb1 = res[i/8] >> 7; //Armazenando o bit mais significativo
        res[i/8] = res[i/8] << 1;

        for (int j = i/8 + 1; j < NUM_BITS/8; j++) //Deslocando os outros elementos
        {   
            msb2 = res[j] >> 7; //Armazenando o bit mais significativo
            res[j] = res[j] << 1;
            res[j] |= msb1; //Trocando o bit menos significativo por msb1
            msb1 = msb2;
        }
    }
}

/* res = a >> n (lógico)*/
void big_shr(BigInt res, BigInt a, int n)
{
    unsigned char lsb1;
    unsigned char lsb2;

    big_copy(res, a);
    if (n >= NUM_BITS)
        return;

    for (int i = 0; i < n; i++)
    {
        lsb1 = 0;

        for (int j = ((NUM_BITS - 8 )/8)-i/8; j >= 0; j--)
        {   
            lsb2 = res[j] << 7; //Armazenando o bit menos significativo
            res[j] = res[j] >> 1;
            res[j] |= lsb1; //Trocando o bit mais significativo por lsb1
            lsb1 = lsb2;   
        }
    }
}

/* res = a >> n (aritmético)*/
void big_sar(BigInt res, BigInt a, int n)
{
    unsigned char lsb1;
    unsigned char lsb2;

    big_copy(res, a);
    if (n >= NUM_BITS)
        return;

    for (int i = 0; i < n; i++)
    {
        lsb1 = 0;
        
        for (int j = (NUM_BITS  -8 -i)/8; j >= 0; j--)
        {   
            lsb2 = res[j] << 7; //Armazenando o bit menos significativo
            if (j == (NUM_BITS  -8 -i)/8)
                res[j] = (signed char) res[j] >> 1;
            else
                res[j] >>= 1; 

            res[j] |= lsb1; //Trocando o bit mais significativo por lsb1
            lsb1 = lsb2;
        }
    }
}