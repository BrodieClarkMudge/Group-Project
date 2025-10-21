# make
# ./betterfarm
# works on WSL / LINUX and MacOS



# For this cross platform related makefiles, since it was not included in course makefile description
# nor was there anything clear online on how to run raylib on different OS's,
# ChatGPT was used from the next line to endif

# Detect OS and use correct Raylib link flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif
ifeq ($(UNAME_S),Darwin)  # macOS
    LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif

# Build game 
betterfarm: *.cpp
	g++ -std=c++17 *.cpp -o betterfarm $(LDFLAGS)

# Run game
run: betterfarm
	./betterfarm

# Clean - delete output files
clean:
	rm -f betterfarm
