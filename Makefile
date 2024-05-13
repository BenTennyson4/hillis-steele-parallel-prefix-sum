# Makefile for PrefixSum Program

 # Compiler settings - Can be customized.
CC = g++ # or gcc, depending on your program
CFLAGS = -Wall -g # Wall for all warnings, g for debugging

# Target executable name
TARGET = my-count

# Source files
SOURCES = my-count.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
#
# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
        $(CC) $(CFLAGS) -o $@ $^

# To obtain object files
%.o: %.cpp
        $(CC) $(CFLAGS) -c $< -o $@

# To remove generated files
clean:
        rm -f $(TARGET) $(OBJECTS)
