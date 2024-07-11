default:
	g++ main.cpp -I include/ -L library/ -o Snake.exe -O2 -Wall -Wno-missing-braces  -lraylib -lopengl32  -lgdi32 -lwinmm -flto -static

linux:
	g++ -I include/ -L library/ main.cpp states.cpp -o Snake -O2 -Wall -lraylib -lGL -lm -lpthread -ldl -lrt -lX11