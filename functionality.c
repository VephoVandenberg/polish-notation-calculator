#include "functionality.h"

// I require that these global variables
// won't be accesseble by other files

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


