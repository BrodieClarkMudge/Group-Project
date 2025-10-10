test: animals.cpp
	clang++ animals.cpp -o game \
    -std=c++17 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    -lraylib \
    -framework Cocoa \
    -framework IOKit \
    -framework CoreVideo \
    -framework OpenGL
	./game

square : raylibSquare.cpp
	clang++ raylibSquare.cpp -o square \
    -std=c++17 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    -lraylib \
    -framework Cocoa \
    -framework IOKit \
    -framework CoreVideo \
    -framework OpenGL
	./square