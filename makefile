CXX = g++
CXXFLAGS = -g -std=c++11 -O3 -Wall
LDFLAGS = -lm
TARGET = src/max-connections-chrono

# List all input files in the tests folder
INPUT_FILES = $(wildcard testsTime/*.in)

# Create a list of corresponding output files
OUTPUT_FILES = $(patsubst %.in, %.out, $(INPUT_FILES))

# Default target
all: $(TARGET) $(OUTPUT_FILES)

# Compile the main program only if max-connections-chrono does not exist
$(TARGET): src/max-connections-chrono.cpp
	@if [ ! -f $(TARGET) ]; then \
		$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS); \
	fi

# Rule to run the program on each input file and redirect the output to .out files
%.out: %.in $(TARGET)
	./$(TARGET) < $< > $@ 2>&1

# Clean up compiled files and generated output files
clean:
	rm -f $(TARGET) $(OUTPUT_FILES)