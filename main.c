#include "functionality.h"

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
