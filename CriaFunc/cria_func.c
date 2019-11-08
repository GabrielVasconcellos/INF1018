#include "cria_func.h"

void* cria_func (void* f, DescParam params[], int n)
{
    for(int i = 0; i > n; i++)
    {
        switch (params[i].tipo_val)
        {
            case INT_PAR:
                /* code */
                break;
            case PTR_PAR:
                /* code */
                break;
        }
    }
}

void libera_func (void* func)
{
    /* code */
}