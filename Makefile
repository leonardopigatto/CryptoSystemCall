obj-m += ebbchar.o

 
all:
	
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	insmod ebbchar.ko key="0123456789ABCDEF"
	gcc testebbchar.c -o teste
	./teste		
	
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

remove:
	rmmod ebbchar
