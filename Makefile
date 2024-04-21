CC=nvcc

ifeq ($(RELEASE), TRUE)
	CFLAGS=-O3 -std=c++17 -Xcompiler -fcoroutines
else
	CFLAGS=-g -G -std=c++17 -Xcompiler -Wall -Xcompiler -Wextra -Xcompiler -fcoroutines
endif

ifeq ($(OS), Windows_NT)
	DEL=del /Q
else
	DEL=rm -f -v
endif

all: linux windows doc

linux: bin/xva.out

windows: bin/xva.exe

# Linux

bin/xva.out: obj/main.o obj/cuda_utils.o obj/pch.o
	$(CC) $(CFLAGS) -o $@ $^

obj/main.o: src/main.cpp headers/cuda_utils.h
	$(CC) $(CFLAGS) -o $@ -c $<

obj/cuda_utils.o: src/cuda_utils.cu headers/cuda_utils.h headers/pch.h
	$(CC) $(CFLAGS) -o $@ -c $<

obj/pch.o: src/pch.cpp headers/pch.h
	$(CC) $(CFLAGS) -o $@ -c $<

# Windows

bin/xva.exe: obj/main.obj obj/cuda_utils.obj obj/pch.obj
	$(CC) $(CFLAGS) -o $@ $^

obj/main.obj: src/main.cpp headers/cuda_utils.h
	$(CC) $(CFLAGS) -o $@ -c $<

obj/cuda_utils.obj: src/cuda_utils.cu headers/cuda_utils.h headers/pch.h
	$(CC) $(CFLAGS) -o $@ -c $<

obj/pch.obj: src/pch.cpp headers/pch.h
	$(CC) $(CFLAGS) -o $@ -c $<

doc:
	doxygen Doxyfile

clean:
	$(DEL) obj/*.o* bin/xva.*