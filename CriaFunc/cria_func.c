#include "cria_func.h"

typedef enum {RDI, RSI, RDX} reg;

unsigned char* insertCode(unsigned char *codigo, char *s)
{	
	for(; *(s-1); s += 2)
	{
		*codigo = 0x0;
		for(int j = 0; j < 2; j++)
		{
			if(s[j] <= '9')
				*codigo = *codigo * 16 + (s[j] - '0');
			else
				*codigo = *codigo * 16 + (s[j] - 'a' + 10);
		}
		codigo++;
	}
	return codigo;
}

void* cria_func (void* f, DescParam params[], int n)
{
	unsigned char *codigo = malloc(sizeof(unsigned char) * 100);
	unsigned char *start = codigo;
	reg arg = RDI;
	reg f_arg = RDI;
	
	codigo = insertCode(codigo, "554889e54883ec2048897c24e048897424e848895424f0");
	
    for(int i = 0; i < n; i++, arg++)
    {
        switch (params[i].orig_val)
        {
            case PARAM:
                switch(params[i].tipo_val)
				{
					case INT_PAR:
                		switch(f_arg)
                		{
                			case RDI:
                				switch(arg)
                				{
                					case RSI:
                						codigo = insertCode(codigo, "8b7424e0");
                						break;
                					case RDX:
                						codigo = insertCode(codigo, "8b5424e0");
                						break;
                				}
                				break;
                			case RSI:
                				switch(arg)
                				{
                					case RDX:
                						codigo = insertCode(codigo, "8b5424e8");
                						break;
                				}
                				break;
                			case RDX:
                				break;
                		}
                		break;
					case PRT_PAR:
						switch(f_arg)
		            		{
		            			case RDI:
		            				switch(arg)
		            				{
		            					case RSI:
		            						codigo = insertCode(codigo, "488b7424e0");
		            						break;
		            					case RDX:
		            						codigo = insertCode(codigo, "488b5424e0");
		            						break;
		            				}
		            				break;
		            			case RSI:
		            				switch(arg)
		            				{
		            					case RDX:
		            						codigo = insertCode(codigo, "488b5424e8");
		            						break;
		            				}
		            				break;
		            			case RDX:
		            				break;
		            	}
		            	break;
				}
				f_arg++;
                break;
            case FIX:
                switch(params[i].tipo_val)
                {
                	case INT_PAR:
                		switch(arg)
                		{
                			case RDI:
                				break;
                			case RSI:
                				break;
                			case RDX:
                				break;
                		}
                		break;
                	case PTR_PAR:
                		break;
                }
                break;
			case IND:
                /* code */
                break;
        }
    }
}

void libera_func (void* func)
{
    /* code */
}
