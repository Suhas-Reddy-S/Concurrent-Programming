CXX = g++

CXXFLAGS = -std=c++11 -g -Wall -Werror -O0

TARGET = mysort

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	rm -rf $(TARGET)
