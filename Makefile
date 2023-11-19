CC = g++
CFLAGS = -O3 -fopenmp -march=native -mtune=native -mavx2 -mfma -m64 -DNDEBUG

SOURCES = src/main.cpp
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(EXECUTABLE)

.PHONY: clean
