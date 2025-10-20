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

farm : betterfarm.cpp pig.cpp sheep.cpp cow.cpp chicken.cpp animal.cpp menu.cpp options.cpp grid.cpp shop.cpp ui.cpp
	clang++ betterfarm.cpp pig.cpp sheep.cpp cow.cpp chicken.cpp animal.cpp menu.cpp options.cpp grid.cpp shop.cpp ui.cpp -o farm \
    -std=c++17 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    -lraylib \
    -framework Cocoa \
    -framework IOKit \
    -framework CoreVideo \
    -framework OpenGL
	./farm