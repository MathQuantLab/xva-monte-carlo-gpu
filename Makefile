CC=nvcc

ifeq ($(RELEASE), TRUE)
	CFLAGS=-O3 -std=c++17
else
	CFLAGS=-g -G -std=c++17
endif

all: bin/xva

bin/xva: obj/main.o
	$(CC) $(CFLAGS) -o xva $^

obj/main.o: src/main.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

doc:
	doxygen Doxyfile