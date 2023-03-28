main:	main.o	command.o	token.o	
	gcc	main.o	command.o	token.o	
	
main.o:	main.c	command.h	token.h	
	gcc	-c	main.c

command.o:	command.c	command.h
	gcc	-c	command.c

token.o:	token.c	token.h
	gcc	-c	token.c

clean:
	rm	*.o
		
