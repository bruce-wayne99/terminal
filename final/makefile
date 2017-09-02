all :
	gcc -c cd.c -o cd.o
	gcc -c command.c -o command.o
	gcc -c ls.c -o ls.o
	gcc -c echo.c -o echo.o
	gcc -c pwd.c -o pwd.o
	gcc -c execute.c -o execute.o
	gcc -c display.c -o display.o
	gcc -c pinfo.c -o pinfo.o
	gcc -c nightswatch.c -o nightswatch.o
	gcc -c main.c -o main.o

	gcc *.o -o run

clean :
	rm -rf *.o
	rm -rf run
