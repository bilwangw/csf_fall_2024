CC = gcc
FLAGS = -Wall -Wextra -pedantic

MAIN = main.c

csim : $(MAIN_OBJS)
	$(CC) $(FLAGS) $(MAIN) -o $@ $+

clean :
	rm -f *.o csim 