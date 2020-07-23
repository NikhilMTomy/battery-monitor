CC:=gcc
FLAGS:=`pkg-config --cflags --libs libnotify`
OUTPUT:=build/battery-monitor
SOURCE:=src/main.c
BINARY:=/usr/local/bin/battery-monitor

all: $(SOURCE)
	$(CC) $(SOURCE) -o $(OUTPUT) $(FLAGS)

install:
	cp $(OUTPUT) $(BINARY)

uninstall:
	rm $(BINARY)

clean:
	rm $(OUTPUT)
