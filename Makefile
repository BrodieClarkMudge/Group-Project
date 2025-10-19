CXX = clang++
CXXFLAGS = -std=c++17 -I. -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit

SRCS = testing.cpp WeatherSystem.cpp Spring.cpp Summer.cpp Autumn.cpp Winter.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = weatherbox

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(SRCS) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o