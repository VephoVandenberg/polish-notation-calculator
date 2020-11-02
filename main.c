#include <stdio.h>
#include "helpers.h"


typedef struct
{
    double stack[MAX_LEN];
    uint8 stack_pointer;
} stack_t;

typedef struct
{
    char buffer[MAX_LEN];
    uint8 buffer_pointer;
} buffer_t;

typedef struct
{
    char mnemonic;
    double  value;
    int8 is_set;
}variable_t;

typedef struct
{
    variable_t variables[MAX_LEN];
    uint8 border;
}variables_space_t;



double pow(int32 number, int32 power);
double to_float(char *string);

void push(double number);
double pop(void);
void swap_top(void);
void print_top(void);
int32 get_operation(char *string);
int32 getch(void);
void ungetch(char character);

int8 isdigit(char character);
void initialize_objects(void);

int8 isvariable(char character);
variable_t *declare_variable(char mnemonics);
void set_variable(char mnemonics, double value);
variable_t *get_variable(char mnemonics);

void clear_all(void);

int main(int argc, char **argv)
{
    initialize_objects(); // I am doing this to set stack size and buffer_size
    
    int32 type;
    double operator1, operator2;
    double value;
    char string[MAX_LEN];
    variable_t *variable1 = NULL;
    variable_t *variable2 = NULL;
    
    printf("\nThat is implementation of reverse polish calculator.\n");
    printf("You can declare variables, as well.\n");
    printf("Restrictions for variable declaration are next:\n");
    printf("\t 1) First goes a variable, for example: A 2 =, c 3 6 + 2 - =.\n");
    printf("\t 2) You may declare a variable using 1 character, from a-z and A-Z.\n\n");
    printf("You can perform following operations.\n");
    printf("\t Simple mathematical expressions - '+ -  * / '. \n");
    printf("And some extra things:\n");
    printf("\t 1) Swap top 2 elements - '$'\n");
    printf("\t 2) Print top element without poping - '#'\n");
    printf("\t 3) Clear every data structure we have - '~'\n");
    printf("\t 4) Duplicate top element of the stack - ':'\n");
    printf("If you want to quit, print 'q' or 'Q'.(P.S. You  cannot declare variables with these names\n");
    printf("To pop out elements from the stack just hit Enter.\n\n");
    
    while ((type = get_operation(string)) != EOF)
    {
	switch(type)
	{
	    case NUMBER:
	    {
		push(to_float(string));
	    }break;

	    case VAR_DETECTED:
	    {
		if (!variable1)
		{
		    variable1 = get_variable(string[0]);
		}
		
		if (variable1)
		{
		    variable2 = get_variable(string[0]);
		    push(variable2->value);
		}
		else
		{
		    variable1 = declare_variable(string[0]);
		}
	    }break;

	    case '+':
	    {	     	       
		push(pop() + pop());
	    }break;

	    case '-':
	    {
		operator2 = pop();
		push(pop() - operator2);
	    }break;

	    case '/':
	    {
		operator2 = pop();
		if (operator2 != 0.0)
		{
		    push(pop() / operator2);
		}
		else
		{
		    printf("Error: zero division detected.\n");
		}
	    }break;

	    case '*':
	    {	       
		push(pop() * pop());
	    }break;

	    case '%':
	    {
		operator2 = pop();
		operator1 = pop();
		if (operator2 != 0.0)
		{
		    push(operator1 - operator2 * (int)(operator1 / operator2));
		}
		else
		{
		    printf("Error: zero division detected.\n");
		}
	    }break;

	    case '=':
	    {
		if (variable1)
		{
		    value = pop();
		    variable1->is_set = 0;
		}
	    }break;

	    case '$': // swap top 2 elements
	    {
		swap_top();
	    }break;

	    case '#': // print top element without poping
	    {
		print_top();
	    }break;

	    case '~': // clear the stack;
	    {
		clear_all();
	    }break;

	    case ':': // duplicate top element
	    {
		value = pop();
		push(value);
		push(value);
	    }break;
	    
	    case '\n':
	    {
		if (variable1)
		{
		    if (!variable1->is_set)
		    {
			set_variable(variable1->mnemonic, value);
			variable1 = NULL;
			variable2 = NULL;
		    }
		    else
		    {
			printf("%f\n", pop());
		    }
		}
		else
		{		    
		    printf("%f\n", pop());
		}
	    }break;

	    case 'Q':
	    case 'q':
	    {
		break;
	    }

	    default:
	    {
		printf("Error: Uknown command\n");
	    }break;
	}

	if (type == 'q' || type == 'Q')
	{
	    break;
	}
    }
    
    return 0;
}


// I require that these objects
// shouldn't be accesseble by main()

global stack_t stack_buffer;
global buffer_t input_buffer;
global variables_space_t variables;


double to_float(char *string)
{
    int32 i;
    int8  sign, magnitude_sign;
    double value, power, magnitude;

    for (i = 0; string[i] == ' '; i++);

    sign = (string[i] == '-') ? -1 : 1;
    if (string[i] == '+' || string[i] == '-')
    {
	i++;
    }

    for (value = 0; isdigit(string[i]); i++)
    {
	value = value * 10 + (string[i] - '0');
    }

    if (string[i] == '.')
    {
	i++;
    }

    for (power = 1; isdigit(string[i]); i++)
    {
       	value = value * 10 + (string[i] - '0');
	power *= 10;
    }

    if (string[i] == 'e' || string[i] == 'E')
    {
	i++;
    }

    magnitude_sign = ('-' == string[i]) ? -1 : 1; 
    if (string[i] == '+' || string[i] == '-')
    {
	i++;
    }
    
    for (magnitude = 0; isdigit(string[i]); i++)
    {
	magnitude = magnitude * 10 + (string[i] - '0');
    }

    if (magnitude_sign == 1)
    {
	return pow(10, magnitude) * sign * (value / power);
    }
    else
    {
	return sign * (value / (power * pow(10, magnitude)));
    }
}

double  pow(int32 number, int32 power)
{
    int32 i;
    int32 result;
    
    for (result = 1, i = 0; i < power; i++)
    {
	result *= number;
    }

    if (power >= 0)
    {
	return result;
    }
    else
    {
	return (1 / result);
    }
}

int32 get_operation(char *filled_string)
{
    int32 i;
    char character;
    
    while ((filled_string[0] = character = getch()) == ' ' || character == '\t');
    
    filled_string[1] = '\0';
    
    if (!isdigit(character) && character != '.')
    {
	if (isvariable(character))
	{
	    return VAR_DETECTED;
	}
	return character; // In this case we return operator
    }

    i = 0;
    if (isdigit(character))
    {
	while (isdigit(filled_string[++i] = character = getch()));
    }

    if (character == '.')
    {
	while (isdigit(filled_string[++i] = character = getch()));
    }

    filled_string[i] = '\0';

    if (character != EOF)
    {
	ungetch(character);
    }

    return NUMBER; // by default we return an indicator that the number was filled in;
}

void push(double number)
{
    if (stack_buffer.stack_pointer < MAX_LEN)
    {
	stack_buffer.stack[stack_buffer.stack_pointer++] = number;
    }
    else
    {
	printf("Error: stack is full.\n");
    }
}

double pop(void)
{
    if (stack_buffer.stack_pointer > 0)
    {
	return stack_buffer.stack[--stack_buffer.stack_pointer];
    }
    else
    {
	printf("Stack is empty.\n");
	return 0;
    }
}

int32 getch(void)
{
    return (input_buffer.buffer_pointer > 0) ? input_buffer.buffer[--input_buffer.buffer_pointer] : getchar();
}

void ungetch(char character)
{
    if (input_buffer.buffer_pointer <  MAX_LEN)
    {
	input_buffer.buffer[input_buffer.buffer_pointer++] = character;
    }
    else
    {
	printf("Error: too many  characters.\n");
	
    }
}

int8 isdidit(char character)
{
    return (character >= '0' && character <= '9') ? 1 : 0;
}

int8 isvariable(char character)
{
    return ((character >= 'A' && character <= 'Z' || character >= 'a' && character <= 'z') && character != 'q' && character != 'Q') ? 1 : 0;
}

void initialize_objects(void)
{    
    stack_buffer.stack_pointer = 0;
    input_buffer.buffer_pointer = 0;
    variables.border = 0;
}

variable_t *declare_variable(char character)
{
    if (variables.border < MAX_LEN)
    {
	variables.variables[variables.border].mnemonic = character;
	variables.border++;
	return &variables.variables[variables.border - 1];
    }
    else
    {
	printf("Error: variables' space is full.\n");
	return NULL;
    }
}


variable_t *get_variable(char mnemonics)
{
    int i;
    
    for (i = 0; i < variables.border; i++)
    {
	if (variables.variables[i].mnemonic == mnemonics)
	{
	    return &variables.variables[i];
	}
    }

    return NULL;
}

void set_variable(char mnemonics, double value)
{
    int i;

    for (i = 0; i < variables.border; i++)
    {
	if (variables.variables[i].mnemonic == mnemonics)
	{
	    variables.variables[i].value = value;
	    variables.variables[i].is_set = 1;
	}
    }
}

void clear_all()
{
    stack_buffer.stack_pointer = 0;
    input_buffer.buffer_pointer = 0;
    variables.border = 0;
}

void swap_top()
{
    double temp;

    if (stack_buffer.stack_pointer > 1 && stack_buffer.stack_pointer < MAX_LEN)
    {
	temp = stack_buffer.stack[stack_buffer.stack_pointer - 2];
	stack_buffer.stack[stack_buffer.stack_pointer - 2] = stack_buffer.stack[stack_buffer.stack_pointer - 1];
	stack_buffer.stack[stack_buffer.stack_pointer - 1] = temp;
    }
    else
    {
	printf("Error: swapping is not possible.\n");
    }
}

void print_top()
{
    if (stack_buffer.stack_pointer > 0);
    {
	printf("%f\n", stack_buffer.stack[stack_buffer.stack_pointer - 1]);
    }
}

