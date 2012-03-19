all: filters.o machine_learning.o
	echo "almost there!"
	g++ `pkg-config --cflags opencv` machine_learning.o filters.o -O2 -o machine_learning `pkg-config --libs opencv`
debug: filters.o machine_learning.o
	echo "Done!"
	g++ `pkg-config --cflags opencv`  machine_learning.o filters.o -O2 -o machine_learning_debug `pkg-config --libs opencv`
machine_learning.o: machine_learning.cpp
	echo hi!
	g++ -c `pkg-config --cflags opencv` machine_learning.cpp -O2 
filters.o: filters.cpp filters.hpp
	echo hi!2
	g++ -c `pkg-config --cflags opencv` filters.cpp -O2 
frame_colors:	
	g++ `pkg-config --cflags opencv` frame_colors.cpp -O2 -o frame_colors `pkg-config --libs opencv`
low_pass: filters.o low_pass.cpp
	g++ `pkg-config --cflags opencv` low_pass.cpp filters.o -O2 -o low_pass `pkg-config --libs opencv`
high_pass: filters.o high_pass.cpp
	g++ `pkg-config --cflags opencv` high_pass.cpp filters.o -O2 -o high_pass  `pkg-config --libs opencv`

