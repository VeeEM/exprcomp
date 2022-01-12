
typedef int Operator;

typedef struct _expression {
  char type;
  long long value;
  struct _expression *left, *right;
  Operator oper;
} Expression;

typedef Expression AST_node;

extern int Parse_program(AST_node **);
