# make
# ./betterfarm
# works on WSL / LINUX and MacOS   

# For this cross platform related makefiles, since it was not included in course makefile description
# nor was there anything clear online on how to run raylib on different OS's,
# ChatGPT was used from the next line to endif

# Detect OS and use correct Raylib link flags
UNAME_S := $(shell uname -s)
# Detect architecture
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_S),Linux)
    CFLAGS = -I/usr/local/include
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif
ifeq ($(UNAME_S),Darwin)  # macOS
    ifeq ($(UNAME_M),arm64)
        # Apple Silicon
        CFLAGS = -I/opt/homebrew/include
        LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
        ARCH_FLAG = -arch arm64
    else ifeq ($(UNAME_M),x86_64)
        # Intel macOS
        CFLAGS = -I/usr/local/include -I/opt/homebrew/include
        LDFLAGS = -L/usr/local/lib -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
        ARCH_FLAG = -arch x86_64
    endif
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