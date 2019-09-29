#include <limits.h>
#include "bigint.h"
#include <stdio.h>

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

/* Funcão Auxiliar */

int big_isZero(BigInt a) {
    int i;
    for (i=0; i < (NUM_BITS/8) - 1; i++) {
        if (a[i] != 0)
            return 0;
    }
    return 1;
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
        else
            curr = 0;
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
    big_val(res, 0);  // initialize result
    int sig = 0;
    if ((a[(NUM_BITS/8)-1] >> 7)&0x1){
        sig = 1;
        big_comp2(a, a);
    }
    if ((b[(NUM_BITS/8)-1] >> 7)&0x1){
        sig ^= 1;
        big_comp2(b, b);
    } 
    while (!big_isZero(b)) 
    { 
        // If second number becomes odd, add the first number to result 
        if (b[0]&0x1) 
            big_sum(res, res, a); 
        // Double the first number and halve the second number 
        big_shl(a, a, 1); 
        big_shr(b, b, 1);
     }
     if(sig)
        big_comp2(res, res); 
}   

/* Operações de Deslocamento */

/* res = a << n */
void big_shl(BigInt res, BigInt a, int n)
{   
    int index = ((NUM_BITS-1) - n) / 8;
    int bit = ((NUM_BITS-1) - n) % 8;
    int indexRes = NUM_BITS/8;
    int bitRes = 3;
    int i;
    for (i=0; i < NUM_BITS/8; i++)
        res[i] = 0;
    if (n >= 128)
        return;
    while(index >= 0) {
        while(bit >= 0) {
            if (bitRes < 0) {
                indexRes--;
                bitRes = 3;
            }
            res[indexRes] |= a[index] << (bitRes - bit);
            bit--;
        }
        index--;
        bit = 3;
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
        lsb1 = res[(NUM_BITS - i)/8] << 7; //Getting the least significant byte

        printf("%02x\n", lsb1);
        printf("res[%d] = %02x\n", (NUM_BITS - i)/8, res[(NUM_BITS - i)/8]);

        res[(NUM_BITS - i)/8] = res[(NUM_BITS - i)/8] >> 1;

        printf("res[%d] = %02x\n", (NUM_BITS - i)/8, res[(NUM_BITS - i)/8]);
        
        for (int j = (NUM_BITS - i)/8 + 1; j < NUM_BITS/8; j++) //Shifting the other elements 
        {   
            lsb2 = res[j] << 7; //Getting the least significant bit

            printf("%02x\n", lsb2);

            printf("res[%d] = %02x\n", j, res[j]);

            res[j] = res[j] >> 1;

            printf("res[%d] = %02x\n", j, res[j]);
 
            res[j] |= lsb1; //Setting the least significant bit to lsb1

            printf("res[%d] = %02x\n", j, res[j]);

            lsb1 = lsb2;
        }
    }
    
}

/* res = a >> n (aritmético)*/
void big_sar(BigInt res, BigInt a, int n)
{
    if(n == 0)
        res = a;
    for(int i = 0; i < n; i++)
    {
        
    }
}