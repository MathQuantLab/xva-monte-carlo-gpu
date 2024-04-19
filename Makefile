CC=nvcc

ifeq ($(RELEASE), TRUE)
	CFLAGS=-O3 -std=c++17
else
	CFLAGS=-g -G -std=c++17
endif

ifeq ($(OS), Windows_NT)
	DEL=del /Q
else
	DEL=rm -f
endif

all: linux windows doc

linux: bin/xva

windows: bin/xva.exe

bin/xva: obj/main.o
	$(CC) $(CFLAGS) -o $@ $^

obj/main.o: src/main.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

bin/xva.exe: obj/main.obj
	$(CC) $(CFLAGS) -o $@ $^

obj/main.obj: src/main.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

doc:
	doxygen Doxyfile

clean:
	$(DEL) obj/*.o* bin/xva bin/xva.exe
	$(DEL) -r -v Documentation/*