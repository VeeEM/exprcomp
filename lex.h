#include <stdlib.h>
#include <stdio.h>

#define EoF 256
#define NUMBER 257

typedef struct {
  int class;
  char repr;
  long long literal;
} Token_type;

extern Token_type Token;
extern void get_next_token();

int init_lexer(FILE *);
void exit_lexer();
