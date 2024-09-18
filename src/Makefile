CC=gcc-10
CFLAGS= -O3 -g
WFLAGS= -Wall
SOURCES=$(shell cat ../sources)
TARGET=huffman

persen: $(SOURCES)
	$(CC) -o $(TARGET) $^ $(CFLAGS)

wall: $(SOURCES)
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(WFLAGS)

debug: $(SOURCES)
	$(CC) -o $(TARGET) $^ $(CFLAGS) -DDEBUG

clean:
	rm $(TARGET)