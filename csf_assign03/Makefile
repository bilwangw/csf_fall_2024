CC = gcc
FLAGS = -Wall -Wextra -pedantic

MAIN = main.c
#MAIN_OBJS = $(MAIN:.c=.o)

#%.o : %.c
#	$(CC) $(FLAGS) -c $*.c -o $*.o

csim : $(MAIN_OBJS)
	$(CC) $(FLAGS) $(MAIN)

clean :
	rm -f *.o csim