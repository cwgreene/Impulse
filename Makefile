all: filters.o machine_learning.o
	echo "almost there!"
	g++ `pkg-config --cflags opencv` machine_learning.o filters.o -O2 -o machine_learning `pkg-config --libs opencv`
machine_learning.o: machine_learning.cpp
	echo hi!
	g++ -c `pkg-config --cflags opencv` machine_learning.cpp -O2
filters.o: filters.cpp filters.hpp
	echo hi!2
	g++ -c `pkg-config --cflags opencv` filters.cpp -O2
debug:
	g++ `pkg-config --cflags opencv` machine_learning.cpp -g -o machine_learning `pkg-config --libs opencv`
frame_colors:	
	g++ `pkg-config --cflags opencv` frame_colors.cpp -O2 -o frame_colors `pkg-config --libs opencv`
