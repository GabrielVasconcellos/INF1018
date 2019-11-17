#include <stdio.h>
#include <stdlib.h>
#include "cria_func.h"

typedef enum {RDI, RSI, RDX} reg;

unsigned char* insertCode(unsigned char *codigo, char *s)
{	
	for(; *(s); s += 2)
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

void hex2string (char *string, void *p, int n)
{
	unsigned char *p1 = p;
	while (n--)
	{
		snprintf(string, 3, "%02x", *p1);
		string+=2;
		p1++;
	}
}

void* cria_func (void* f, DescParam params[], int n)
{
	unsigned char *codigo =(unsigned char *) malloc(sizeof(unsigned char) * 100);
	unsigned char *start = codigo;
	char string[17];
	reg arg = RDI;
	reg f_arg = RDI;
	if (codigo == NULL)
	{
		printf("malloc error\n");
		exit(1);
	}

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
									case RDI:
										break;

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
									case RDI:
										break;

									case RSI:
										break;

                					case RDX:
                						codigo = insertCode(codigo, "8b5424e8");
                						break;
                				}
                				break;
                			case RDX:
                				break;
                		}
                		break;
					case PTR_PAR:
						switch(f_arg)
		            		{
		            			case RDI:
		            				switch(arg)
		            				{
										case RDI:
											break;

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
										case RDI:
											break;
										
										case RSI:
											break;

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
								codigo = insertCode(codigo, "bf");
                				break;
                			case RSI:
								codigo = insertCode(codigo, "be");
                				break;
                			case RDX:
								codigo = insertCode(codigo, "ba");
                				break;
                		}
						hex2string(string,(void *) &params[i].valor.v_int, sizeof(int));
                		break;
                	case PTR_PAR:
						switch (arg)
						{
							case RDI:
								codigo = insertCode(codigo, "48bf");
                				break;
                			case RSI:
								codigo = insertCode(codigo, "48be");
                				break;
                			case RDX:
								codigo = insertCode(codigo, "48ba");
                				break;
						}
						hex2string(string,(void *) &params[i].valor.v_ptr, sizeof(int *));
                		break;
                }
				codigo = insertCode(codigo, string);
                break;
			case IND:
				codigo = insertCode(codigo, "48b9");
				hex2string(string,(void *) &params[i].valor.v_ptr, sizeof(int *));
				codigo = insertCode(codigo, string);
				switch(params[i].tipo_val)
                {
                	case INT_PAR:
                		switch(arg)
                		{
                			case RDI:
								codigo = insertCode(codigo, "8b39");
                				break;
                			case RSI:
								codigo = insertCode(codigo, "8b31");
                				break;
                			case RDX:
								codigo = insertCode(codigo, "8b11");
                				break;
                		}
                		break;
                	case PTR_PAR:
						switch (arg)
						{
							case RDI:
								codigo = insertCode(codigo, "488b39");
                				break;
                			case RSI:
								codigo = insertCode(codigo, "488b31");
                				break;
                			case RDX:
								codigo = insertCode(codigo, "488b11");
                				break;
						}
                		break;
                }
                break;
        }
    }

	codigo = insertCode(codigo, "48b9");
	hex2string(string, &f, sizeof(void *));
	codigo = insertCode(codigo, string);
	codigo = insertCode(codigo, "ffd1c9c3");

	return (void *) start;
}

void libera_func (void* func)
{
    free(func);
}
