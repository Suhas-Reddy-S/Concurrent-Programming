# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = --std=c++2a -g -Wall -Werror -O3 -pthread -latomic

# Source Files
SOURCES = main.cpp queues.cpp stacks.cpp file_copy.cpp

# Header Files
HEADERS = queues.hpp stacks.hpp main.hpp

# Object Files
OBJECTS = $(SOURCES:.cpp=.o)

# Output Executable
TARGET = concurrent_containers

# Default Rule
all: $(TARGET)

# Link Objects to Create Executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile Source Files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Build Artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Rebuild the Project
rebuild: clean all
