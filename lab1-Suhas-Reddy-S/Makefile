CXX = g++

CXXFLAGS = --std=c++2a -g -Wall -Werror -O0 -pthread

# Source files
SRCS = mysort.cpp mergesort.cpp bucketsort.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = mysort

all: $(TARGET)

# Rule to build the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files from source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -rf $(OBJS) $(TARGET)
