#include "lex.h"
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include <math.h>

static int is_whitespace(char c) {
  switch(c) {
    case ' ':
    case '\t':
    case '\n':
      return 1;
    default:
      return 0;
  }
}

Token_type Token;

int current = 0;
int start = 0;

char* source;
int source_len = 0;

// File reading code borrowed from user Nominal Animal on SO. There 
// might be a simpler way to read a file at once, especially as I 
// support only Linux, but I have not yet looked into it.
// https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer/44894946#44894946
int init_lexer(FILE *input) {
  char *datat = NULL, *temp;
  size_t sizet = 0;
  size_t used = 0;
  size_t n;
  
  int const chunk = 262144;
  
  if (input == NULL) {
    Error("init_lexer inputfile == NULL");
  }
  
  if (ferror(input)) {
    Error("init_lexer input file error");
  }
  
  while (1) {
    if (used + chunk + 1 >  sizet) {
      sizet = used + chunk + 1;
      if (sizet <= used) {
	free(datat);
	Error("init_lexer string overflow");
      }
      
      temp = realloc(datat, sizet);
      if (temp == NULL) {
	Error("init_lexer out of memory");
      }
      datat = temp;
    }
    n = fread(datat + used, 1, chunk, input);
    if (n == 0)
      break;
    
    used += n;
  }
  
  if (ferror(input)) {
    free(datat);
    Error("init_lexer input error");
  }
  temp = realloc(datat, used + 1);
  if (temp == NULL) {
    Error("init_lexer out of memory");
  }
  datat = temp;
  datat[used] = '\0';
  
  source = datat;
  source_len = used;
  
  return 0;
}

void exit_lexer() {
  free(source);
}

static char advance() {
  return source[current++];
}

static char peek() {
  return source[current];
}

static long long read_literal() {
  int digit = current - start - 1;
  int n = 0;
  
  for (int i = start; i < current; i++) {
    n += (source[i] - 48) * pow(10, digit);
    digit--;
  }
  return n;
}

static int is_number(char c) {
  return '0' <= c && c <= '9';
}

void get_next_token() {
  char c;
  
  do {
    start = current;
    c = advance();
    if (c == 0) {
      Token.class = EoF;
      Token.repr = '#';
      return;
    }
  } while(is_whitespace(c));
  
  if (is_number(c)) {
    Token.class = NUMBER;
    while(is_number(peek())) advance();
    Token.literal = read_literal();
  } else {
    Token.class = c;
  }
  Token.repr = c;
}
