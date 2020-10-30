#include <stdio.h>
#include "helpers.h"


double pow(int32 number, int32 power);
double to_float(char *string);
int32 to_integer(char *string);
void push(double number);
double pop(void);
int32 get_operation(char *string);
int32 getch(void);
void ungetch(char character);
int8 isdigit(char character);


typedef struct
{
    double stack[MAX_LEN];
    uint8 stack_pointer;
} stack_t;

typedef struct
{
    char buffer[BUFFER_SIZE];
    uint8 buffer_pointer;
} buffer_t;

global stack_t stack_buffer;
global buffer_t input_buffer;


int main(int argc, char **argv)
{
    stack_buffer.stack_pointer = 0;
    input_buffer.buffer_pointer = 0;

    int32 type;
    double operator2;
    char string[MAX_LEN];

    printf("If you want to quit print 'q' or 'Q'.\n");
    printf("To pop out elements from the stack just hit Enter.\n");
    
    while ((type = get_operation(string)) != EOF)
    {
	switch(type)
	{
	    case NUMBER:
	    {
		push(to_float(string));
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
		    printf("Error: null division detected.\n");
		}
	    }break;

	    case '*':
	    {
		push(pop() * pop());
	    }break;

	    case '\n':
	    {
		printf("%f\n", pop());
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

int32 to_integer(char *string)
{
    int32 number;
    int8 i, sign;

    for (i = 0; string[i] == ' '; i++);

    sign = (string[i] == '-') ? -1 : 1;
    if (string[i] == '+' || string[i] == '-')
    {
	i++;
    }

    for (number = 0; isdigit(string[i]); i++)
    {
	number = number * 10 + (string[i] - '0');
    }
    return sign * number;
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
	printf("Stack is empty\n");
	return 0;
    }
}

int32 getch(void)
{
    return (input_buffer.buffer_pointer > 0) ? input_buffer.buffer[--input_buffer.buffer_pointer] : getchar();
}

void ungetch(char character)
{
    if (input_buffer.buffer_pointer <  BUFFER_SIZE)
    {
	input_buffer.buffer[input_buffer.buffer_pointer++] = character;
    }
    else
    {
	printf("too many: characters");
	
    }
}

int8 isdidit(char character)
{
    return (character >= '0' && character <= '9') ? 1 : 0;
}

