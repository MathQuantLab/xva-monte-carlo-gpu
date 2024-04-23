
ifeq ($(RELEASE), TRUE)
	CFLAGS=-O3 -std=c++17 -s -pthread
else
	CFLAGS=-std=c++17 -Wall -Wextra -DDEBUG -pthread
endif
BUILD_DIR = build

# Main Library
SOURCES = $(wildcard internal/*.cpp)

all: libcsv.a

libcsv.a:
	make csv
	ar rvs ../Libs/$@ $(wildcard build/*.o)

csv:
	g++ -c $(CFLAGS) $(SOURCES)
	mkdir -p $(BUILD_DIR)
	mv *.o $(BUILD_DIR)