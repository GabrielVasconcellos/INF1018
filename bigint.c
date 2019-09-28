#include "bigint.h"
#include <stdio.h>

/* Atribuição (com extensão) */
void big_val (BigInt res, long val)
{

}

/* Operações Aritméticas */

/* res = -a */
void big_comp2(BigInt res, BigInt a)
{

}

/* res = a + b */
void big_sum(BigInt res, BigInt a, BigInt b)
{

}

/* res = a - b */
void big_sub(BigInt res, BigInt a, BigInt b)
{

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