all:
	g++ `pkg-config --cflags opencv` machine_learning.cpp -O2 -o machine_learning `pkg-config --libs opencv`
debug:
	g++ `pkg-config --cflags opencv` machine_learning.cpp -g -o machine_learning `pkg-config --libs opencv`
frame_colors:	
	g++ `pkg-config --cflags opencv` frame_colors.cpp -O2 -o frame_colors `pkg-config --libs opencv`
