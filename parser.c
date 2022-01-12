#include <stdlib.h>
#include "lex.h"
#include "error.h"
#include "parser.h"

static Expression *new_expression() {
  return (Expression *)malloc(sizeof(Expression));
}

static Expression *parse_expression();
static Expression *parse_factor();
static Expression *parse_constant();

static Expression *parse_expression() {
  Expression *expr = parse_factor();
  
  while (Token.class == '+') {
    get_next_token();
    Expression *expr_add = new_expression();
    expr_add->type = 'P';
    expr_add->oper = '+';
    expr_add->left = expr;
    expr_add->right = parse_factor();
    expr = expr_add;
  }
  return expr;
}

static Expression *parse_factor() {
  Expression *expr = parse_constant();
  
  while (Token.class == '*') {
    get_next_token();
    Expression *expr_mult = new_expression();
    expr_mult->type = 'P';
    expr_mult->oper = '*';
    expr_mult->left = expr;
    expr_mult->right = parse_constant();
    expr = expr_mult;
  }
  return expr;
}

static Expression *parse_constant() {
  Expression *expr = NULL;
  
  if (Token.class == NUMBER) {
    get_next_token();
    expr = new_expression();
    expr->type = 'D';
    expr->value = Token.literal;
    return expr;
  }
  
  if (Token.class == '(') {
    get_next_token();
    expr = parse_expression();
    if (Token.class != ')') Error("Missing closing )");
    get_next_token();
  }
  
  if (expr == NULL) Error("Expected expression or constant");

  return expr;
}

int Parse_program(AST_node **icode_p) {
  Expression *expr;

  get_next_token();
  expr = parse_expression();
  if (expr) {
    if (Token.class != EoF) {
      Error("Garbage after end of program");
    }
    *icode_p = expr;
    return 1;
  }
  return 0;
}
