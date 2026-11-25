all: uecsrxdump

uecsrxdump: rx.o ccm.o
	cc -o uecsrxdump rx.o ccm.o

rx.o: rx.c ccm.h
	cc -c rx.c
ccm.o: ccm.c ccm.h
	cc -c ccm.c

install:
	install uecsrxdump /usr/local/bin/

clean:
	-/bin/rm *.o uecsrxdump *~
