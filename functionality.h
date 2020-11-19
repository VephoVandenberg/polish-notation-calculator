#include "helpers.h"
#include <stdio.h>

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

