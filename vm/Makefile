CC=clang
#LIBS=-lcurl
CFLAGS=-c -Wall
#EXTS=zstr/*.c

EXECNAME=vm

all: vm 

test: clean all run

vm: cfiles 
	$(CC) $(LIBS) *.o -o $(EXECNAME)

cfiles:
	$(CC) $(CFLAGS) $(EXTS) *.c

clean:
	rm -rf *.o $(EXECNAME)

run: vm 
	./vm
