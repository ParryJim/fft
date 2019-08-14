# Makefile for demo of fft

CC=g++
CFLAGS=-I.
DEPS = fft.h
OBJ = fft.o fftTest.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

fftTest: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)



clean:
	-rm fft.o fftTest.o fftTest.exe