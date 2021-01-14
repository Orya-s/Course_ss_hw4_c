CC = gcc
AR = ar
FLAGS = -Wall -g


all:  frequency 

frequency: frequency.c 
	$(CC) $(FLAGS) frequency.c -o frequency

	

.PHONY: clean 
	
clean: 
	rm -f *.o frequency 
	
