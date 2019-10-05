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
    int sig = (val >> ((sizeof(val)*8)-1)) & 0x1; /* pega o bit mais significativo de val */
    unsigned char *walker = (unsigned char*) & val;

    for (i = 0;  i < (NUM_BITS/8); i++)
    {
        if (i < sizeof(val)) /* coloca o valor de val em res */
        { 
            res[i] = *walker;
            walker++;
        }
        else if(sig) /* se o numero for negativo completa os bits do vetor com 1 */
            res[i] = 0xff;
        else /* se o numero for positivo completa os bits do vetor com 0 */
            res[i] = 0;
    }
}

/* Operações Aritméticas */

/* res = -a */
void big_comp2(BigInt res, BigInt a)
{
    int i;
    int carry = 1; /* inicializa o carry com o numero 1 para o complemento a dois */

    for (i=0; i < (NUM_BITS/8); i++) {
        res[i] = ~a[i];
        if(carry) { /* enquanto houver carry */ 
            carry += res[i];
            res[i] = carry & 0xff; /* coloca em res apenas os 8 primeiros bits de diff */
            carry >>= 8; /* retira os 8 primeiros bits do carry */
        }
    }
}

/* res = a + b */
void big_sum(BigInt res, BigInt a, BigInt b)
{
    unsigned int carry=0; /* inicializa o carry com zero já que não possui resto */

    for(int i = 0; i < (NUM_BITS/8); i++)
    {
        carry += a[i]+b[i];
        res[i] = carry & 0xff; /* coloca em res os 8 primeiros bits de diff */
        carry >>= 8; /* retira os 8 primeiros bits do carry */
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
    BigInt aAux;
    BigInt bAux;
    int sig = 0;


    /* coloca os BigInts a e b em variáveis auxiliares */
    big_copy(aAux, a);
    big_copy(bAux, b);

    big_val(res, 0);  
    
    /* Verifica se a e b são negativos */
    if ((aAux[(NUM_BITS/8)-1] >> 7) & 0x1)
    {
        sig = 1;
        big_comp2(aAux, aAux);
    }
    if ((bAux[(NUM_BITS/8)-1] >> 7) & 0x1)
    {
        sig ^= 1;
        big_comp2(bAux, bAux);
    } 

    /* Faz a multiplicação de numeros positivos */
    while (!big_isZero(bAux)) /* Enquanto b for diferente de zero */
    { 
        if (bAux[0] & 0x1) /* se b for ímpar */
            big_sum(res, res, aAux); 
        
        /* Dobra o valor de a e faz a metade de b */
        big_shl(aAux, aAux, 1); 
        big_shr(bAux, bAux, 1);
    }
    if(sig) /* se apenas um dos numeros for negativo o resultado da multiplição também será */
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
        msb1 = res[i/8] >> 7; /* Armazenando o bit mais significativo */
        res[i/8] = res[i/8] << 1;

        for (int j = i/8 + 1; j < NUM_BITS/8; j++) /* Deslocando os outros elementos */
        {   
            msb2 = res[j] >> 7; /* Armazenando o bit mais significativo */
            res[j] = res[j] << 1;
            res[j] |= msb1; /* Trocando o bit menos significativo por msb1 */
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
            lsb2 = res[j] << 7; /* Armazenando o bit menos significativo */
            res[j] = res[j] >> 1;
            res[j] |= lsb1; /* Trocando o bit mais significativo por lsb1 */
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
            lsb2 = res[j] << 7; /* Armazenando o bit menos significativo */
            if (j == (NUM_BITS  -8 -i)/8)
                res[j] = (signed char) res[j] >> 1;
            else
                res[j] >>= 1; 

            res[j] |= lsb1; /* Trocando o bit mais significativo por lsb1 */
            lsb1 = lsb2;
        }
    }
}