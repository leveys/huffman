CC=gcc-10
CFLAGS= -O3 -g
WFLAGS= -Wall
SOURCES=$(shell cat test_sources)
TARGET=test

test: $(SOURCES)
	$(CC) -o $(TARGET) $^ $(CFLAGS)

wall: $(SOURCES)
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(WFLAGS)

clean:
	rm $(TARGET)