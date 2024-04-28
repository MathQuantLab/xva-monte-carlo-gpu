CC=nvcc

ifeq ($(RELEASE), TRUE)
	CFLAGS=-O3 -std=c++17 -I./Include
else
	ifeq ($(OS), Windows_NT)
		CFLAGS=-g -G -std=c++17 -I./Include -DDEBUG
	else
		CFLAGS=CFLAGS=-g -G -std=c++17 -Xcompiler -Wall -Xcompiler -Wextra -I./Include -DDEBUG
	endif
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

bin/xva.out: obj/main.o obj/cuda_utils.o obj/pch.o obj/utils.o obj/cuda_simulation.o obj/simulation.o obj/nmc.o
	@echo "Building Linux binary..."
	$(CC) $(CFLAGS) -o $@ $^

obj/main.o: src/main.cpp headers/cuda_utils.h headers/utils.h headers/simulation.h
	@echo "Compiling main.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/cuda_utils.o: src/cuda_utils.cu headers/cuda_utils.h headers/pch.h
	@echo "Compiling cuda_utils.cu..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/pch.o: src/pch.cpp headers/pch.h
	@echo "Compiling pch.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/utils.o: src/utils.cpp headers/cuda_utils.h headers/pch.h headers/utils.h
	@echo "Compiling utils.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/cuda_simulation.o: src/cuda_simulation.cu headers/cuda_simulation.h headers/pch.h
	@echo "Compiling cuda_simulation.cu..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/simulation.o: src/simulation.cpp headers/simulation.h headers/pch.h headers/nmc.h
	@echo "Compiling simulation.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/nmc.o: src/nmc.cpp headers/nmc.h headers/pch.h
	@echo "Compiling nmc.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

# Windows

bin/xva.exe: obj/main.obj obj/cuda_utils.obj obj/pch.obj obj/utils.obj obj/cuda_simulation.obj obj/simulation.obj obj/nmc.obj
	@echo "Building Linux binary..."
	$(CC) $(CFLAGS) -o $@ $^

obj/main.obj: src/main.cpp headers/cuda_utils.h headers/utils.h headers/simulation.h
	@echo "Compiling main.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/cuda_utils.obj: src/cuda_utils.cu headers/cuda_utils.h headers/pch.h
	@echo "Compiling cuda_utils.cu..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/pch.obj: src/pch.cpp headers/pch.h
	@echo "Compiling pch.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/utils.obj: src/utils.cpp headers/cuda_utils.h headers/pch.h headers/utils.h
	@echo "Compiling utils.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/cuda_simulation.obj: src/cuda_simulation.cu headers/cuda_simulation.h headers/pch.h
	@echo "Compiling cuda_simulation.cu..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/simulation.obj: src/simulation.cpp headers/simulation.h headers/pch.h headers/nmc.h
	@echo "Compiling simulation.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/nmc.obj: src/nmc.cpp headers/nmc.h headers/pch.h
	@echo "Compiling nmc.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

doc:
	doxygen Doxyfile

clean:
	$(DEL) obj/*.o* bin/xva.*