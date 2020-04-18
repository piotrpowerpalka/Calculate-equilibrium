main: generuj_model.c helper.o
	gcc -o calcEqu.exe generuj_model.c helper.o

helper.o: helper.c
	gcc -c helper.c

clean:
	rm *.o
	rm calcEqu.exe
