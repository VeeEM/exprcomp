#include "backend.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include "lex.h"

int main(int argc, char **argv) {
  if (argc == 2) {
    FILE *input = fopen(argv[1], "r");
    AST_node *ast;

    if (input == NULL) {
      printf("Could not open file %s\n", argv[1]);
      exit(1);
    }
    init_lexer(input);
    if (!Parse_program(&ast)) Error("No top-level expression");
    exit_lexer();

    printf("\n");
    Print_ast(ast);
    
    FILE *output = fopen("program.s", "w+");
    if (output == NULL) Error("Could not open output file");
    Generate_assembly(output, ast);
    fclose(output);
    
    system("as -o program.o program.s && ld -o program program.o");

  } else {
    printf("Usage: exprcomp <filename>\n");
  }

  return 0;
}
