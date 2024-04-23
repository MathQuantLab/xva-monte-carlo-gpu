CC=nvcc

ifeq ($(RELEASE), TRUE)
	CFLAGS=-O3 -std=c++17 -I./Include
else
	CFLAGS=-g -G -std=c++17 -Xcompiler -Wall -Xcompiler -Wextra -I./Include -DDEBUG
endif

ifeq ($(OS), Windows_NT)
	DEL=del /Q
else
	DEL=rm -f -v
endif

all: linux doc

linux: bin/xva.out

# Linux

bin/xva.out: obj/main.o obj/cuda_utils.o obj/pch.o obj/utils.o obj/cuda_simulation.o Libs/libcsv.a obj/simulation.o obj/nmc.o
	@echo "Building Linux binary..."
	$(CC) $(CFLAGS) -o $@ $^ -L./Libs -lcsv

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

Libs/libcsv.a:
	@echo "Building Linux library..."
	make -C Include libcsv.a RELEASE=$(RELEASE)

obj/simulation.o: src/simulation.cpp headers/simulation.h headers/pch.h headers/nmc.h
	@echo "Compiling simulation.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

obj/nmc.o: src/nmc.cpp headers/nmc.h headers/pch.h
	@echo "Compiling nmc.cpp..."
	$(CC) $(CFLAGS) -o $@ -c $<

doc:
	doxygen Doxyfile

clean:
	$(DEL) obj/*.o* bin/xva.* Libs/*.a