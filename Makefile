
OBJS = main.o lex.o parser.o error.o backend.o
CFLAG = -Wall -g -Wextra -MMD -MP
LDFLAG = -lm
CC = gcc

exprcomp: $(OBJS)
	$(CC) $^ -o $@ $(LDFLAG)

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean:
	-rm exprcomp $(OBJS)

-include $(OBJS:.o=.d)
