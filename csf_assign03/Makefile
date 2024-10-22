CC = g++
FLAGS = -Wall -Wextra -pedantic -g
.PHONY: csim
MAIN = main.cpp

csim : $(MAIN_OBJS)
	$(CC) $(FLAGS) $(MAIN) -o $@ $+

clean :
	rm -f *.o csim 