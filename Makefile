main: main.o token.o command.o commandexecution.o
	gcc main.o token.o command.o commandexecution.o -o myshell

main.o: main.c include/command.h include/token.h include/commandexecution.h
	gcc -c main.c

command.o: src/command.c include/command.h
	gcc -c src/command.c -o command.o	-I../include

token.o: src/token.c include/token.h
	gcc -c src/token.c -o token.o	-I../include

commandexecution.o: src/commandexecution.c include/commandexecution.h
	gcc -c src/commandexecution.c -o commandexecution.o	-I../include

clean:
	rm *.o myshell

		
