#TOPDIR  := $(shell cd ../..; pwd)
#-ld2xx_table
CC=gcc
CFLAGS=-L. -lftd2xx -Wl,-rpath /usr/local/lib

APP = test_canusb

all: $(APP)

$(APP): main.c	
	$(CC) -o $(APP) $(CFLAGS) main.c	

clean:
	rm -f *.o ; rm $(APP)
