obj-m += myserver.o
myserver-objs := myserver.o httpd.o

all:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

#server: main.o httpd.o
#	        gcc -o server $^

#main.o: main.c httpd.h
#	        gcc -c -o main.o main.c

#httpd.o: httpd.c httpd.h
#	        gcc -c -o httpd.o httpd.c
