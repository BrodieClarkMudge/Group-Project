CXX = clang++
CXXFLAGS = -std=c++17 -I. -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit

SRCS = main.cpp \
       grid.cpp \
       menu.cpp \
       options.cpp \
       shop.cpp \
       ui.cpp \
       animal.cpp \
       chicken.cpp \
       cow.cpp \
       sheep.cpp \
       pig.cpp \
       Crop.cpp \
       Regenerating.cpp \
       Stalk.cpp \
       Large.cpp \
       Berry.cpp \
       Potato.cpp \
       Pumpkin.cpp \
       WeatherSystem.cpp \
       Spring.cpp \
       Summer.cpp \
       Autumn.cpp \
       Winter.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = farmgame

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)