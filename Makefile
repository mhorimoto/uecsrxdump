all: uecsrxdump

uecsrxdump: rx.o rep.o
	cc -o uecsrxdump rx.o rep.o

rx.o: rx.c
	cc -c rx.c
rep.o: rep.c
	cc -c rep.c

install:
	install uecsrxdump /usr/local/bin/

clean:
	-/bin/rm *.o uecsrxdump *~
