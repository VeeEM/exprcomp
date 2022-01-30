#include "backend.h"
#include <stdio.h>

static void print_spaces(int n) {
  for (int i = 0; i < n; i++) {
    printf(" ");
    printf(" ");
  }
}

static void print_ast(Expression *expr, int level) {
  switch(expr->type) {
    case 'D':
      print_spaces(level);
      printf("%lld\n", expr->value);
      break;
    case 'P':
      print_spaces(level);
      printf("(%c\n", expr->oper);
      print_ast(expr->left, level + 1);
      print_ast(expr->right, level + 1);
      print_spaces(level);
      printf(")\n");
      break;
  }
}

static void line_left(char* str, FILE *f) {
  fprintf(f, "%s\n", str);
}

static void line_right(char* str, FILE *f) {
  fprintf(f, "\t%s\n", str);
}

static void ast_to_assembly(FILE *f, AST_node *ast) {
  switch(ast->type) {
    case 'D':
      fprintf(f, "\tpushq $%lld\n", ast->value);
      break;
    case 'P':
      // 1 - 2
      ast_to_assembly(f, ast->left);
      ast_to_assembly(f, ast->right);
      switch(ast->oper) {
	case '+':
	  line_right("popq %rax", f);
	  line_right("popq %rbx", f);
	  line_right("addq %rax, %rbx", f);
	  line_right("pushq %rbx", f);
	  break;
	case '*':
	  line_right("popq %rax", f);
	  line_right("popq %rbx", f);
	  line_right("imulq %rax, %rbx", f);
	  line_right("pushq %rbx", f);
	  break;
      }
      break;
  }
}

void Generate_assembly(FILE *f, AST_node *ast) {
  line_left(".global _start", f);
  fputs("\n", f);
  line_left(".text", f);

  line_left("printnumber:", f);
  line_right("lea output, %rsi", f);
  line_right("xor %r8, %r8", f); // Digit counter
  line_left("begin:", f);
  line_right("cqto", f); // Sign-extend rax to rdx for division
  line_right("movq $10, %rbx", f);
  line_right("idivq %rbx", f); // Quotient goes in rax, remainder in rdx
  line_right("addq $48, %rdx", f); // Convert digit into ascii
  line_right("pushq %rdx", f);
  line_right("addq $1, %r8", f);
  line_right("movq $0, %rbx", f);
  line_right("cmpq %rbx, %rax", f);
  line_right("jg begin", f);

  fputs("\n", f);
  
  // Here the characters that were pushed on to the stack previously
  // are popped and stored in memory in the right order.
  line_right("movq %r8, %rdi", f);
  line_left("writeout:", f);
  line_right("popq %rax", f);
  line_right("movb %al, (%rsi, %rbx)", f); // Chars to memory in the right order
  line_right("addq $1, %rbx", f);
  line_right("cmpq %r8, %rbx", f);
  line_right("jl writeout", f);

  fputs("\n", f);

  // Include a newline character at the end of output string
  line_right("movb $10, (%rsi, %rbx)", f);
  line_right("addq $1, %r8", f);

  fputs("\n", f);

  line_right("movq $1, %rax", f); // Write syscall
  line_right("movq $1, %rdi", f); // stdout
  line_right("movq %r8, %rdx", f); // How many bytes to write
  line_right("syscall", f);
  line_right("ret", f);
  
  fputs("\n", f);
  line_left("_start:", f);
  ast_to_assembly(f, ast);
  line_right("popq %rax", f);
  line_right("call printnumber", f);
  
  fputs("\n", f);
  
  line_right("movq $60, %rax", f); // Exit syscall
  line_right("xor %rdi, %rdi", f); // Exit code 0
  line_right("syscall", f);
  
  fputs("\n", f);
  
  line_left(".data", f);
  line_left("output:", f);
  line_right(".zero 32", f);
}

void Print_ast(AST_node *ast) {
  print_ast(ast, 0);
}
