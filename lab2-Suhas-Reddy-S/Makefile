CXX = g++
CXXFLAGS = --std=c++2a -g -Wall -Werror -O3 -pthread #  -fsanitize=address

# Source and header files
COMMON_SRCS = lock.cpp atomic_operations.cpp
COMMON_HDRS = locks.h
TARGET1_SRC = counter.cpp
TARGET2_SRC = mysort.cpp pthread_bucketsort.cpp tas_bucketsort.cpp ttas_bucketsort.cpp tk_bucketsort.cpp pts_bucketsort.cpp bar_bucketsort.cpp mcs_bucketsort.cpp bar_pthread_bucketsort.cpp
TARGET2_HDRS = mysort.h

# Object files
COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
TARGET1_OBJS = $(TARGET1_SRC:.cpp=.o)
TARGET2_OBJS = $(TARGET2_SRC:.cpp=.o)

# Target executables
TARGET1 = counter
TARGET2 = mysort

all: $(TARGET1) $(TARGET2)

# Rule to build the first target executable
$(TARGET1): $(COMMON_OBJS) $(TARGET1_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(COMMON_OBJS) $(TARGET1_OBJS)

# Rule to build the second target executable
$(TARGET2): $(COMMON_OBJS) $(TARGET2_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(COMMON_OBJS) $(TARGET2_OBJS)

# Rule to build object files from source files
%.o: %.cpp $(COMMON_HDRS) $(TARGET2_HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executables
clean:
	rm -rf $(COMMON_OBJS) $(TARGET1_OBJS) $(TARGET2_OBJS) $(TARGET1) $(TARGET2)

# Optional target to run the second executable
run: $(TARGET2)
	./$(TARGET2)

.PHONY: clean all run
