# Compiler
CXX := g++
CXXFLAGS := -g -std=c++17 -D LINUX -fsanitize=address
LDFLAGS := -fsanitize=address

# Includes
INCLUDES := -I./thirdparty/glm -I./include

# Files
SRC := $(wildcard src/*.cpp) $(wildcard utils/*.cpp)
OBJ := $(SRC:.cpp=.o)

# Output
TARGET := main

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(INCLUDES) $(LDFLAGS)

# Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET)