all: rezultdyn

rezultdyn: main.c libfsdyn.so 
	gcc -c main.c
	gcc main.o -o main -ldl
	LD_LIBRARY_PATH=pwd
	./main
libfsdyn.so: f1.c f2.c
	gcc -fPIC -c f1.c f2.c
	gcc -shared -o libfsdyn.so f1.o f2.o

clean:
	rm -rf *.o main *so
