# make
# ./betterfarm
# works on WSL / LINUX and MacOS   
# For this cross platform related makefiles, since it was not included in course makefile description
# nor was there anything clear online on how to run raylib on different OS's,
# ChatGPT was used from the next line to endif
# Detect OS and use correct Raylib link flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CFLAGS = -I/usr/local/include
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else ifeq ($(UNAME_S),Darwin)  # macOS
    CFLAGS = -I/usr/local/include -I/opt/homebrew/include
    LDFLAGS = -L/usr/local/lib -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
    $(error Unsupported OS: $(UNAME_S))
endif
# Build game 
betterfarm: *.cpp
	g++ -std=c++17 $(CFLAGS) *.cpp -o betterfarm $(LDFLAGS)
# Run game
run: betterfarm
	./betterfarm
# Clean - delete output files
clean:
	rm -f betterfarm