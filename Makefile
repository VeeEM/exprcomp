
OBJS = main.o lex.o parser.o error.o backend.o
CFLAGS = -Wall -g -Wextra -MMD -MP
LDFLAGS = -lm
CC = gcc

exprcomp: $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.c.o: %.c
	$(CC) $(CFLAGSS) -c $< -o $@

.PHONY : clean
clean:
	-rm exprcomp $(OBJS)

-include $(OBJS:.o=.d)
