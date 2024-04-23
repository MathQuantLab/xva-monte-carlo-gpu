
CFLAGS=-O3 -std=c++17 -pthread
BUILD_DIR = build

# Main Library
SOURCES = $(wildcard internal/*.cpp)

all: libcsv.a

libcsv.a:
	make csv
	ar rvs ../Libs/$@ $(wildcard build/*.o)

csv:
	$(CXX) -c $(CFLAGS) $(SOURCES)
	mkdir -p $(BUILD_DIR)
	mv *.o $(BUILD_DIR)