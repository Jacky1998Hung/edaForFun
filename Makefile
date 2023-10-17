# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17

# Source files and object files
SRCS = M11215103.cpp 
OBJS = $(SRCS:.cpp=.o)

# Output binary
OUTPUT = mlrcs

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUTPUT)

.PHONY: all clean
