all: filters.o band_pass.o
	g++ -std=c++11 `pkg-config --cflags opencv` band_pass.o filters.o -O2 -o band_pass `pkg-config --libs opencv` -lboost_system -lboost_filesystem -lboost_program_options
debug: filters.o band_pass.o
	g++ `pkg-config --cflags opencv`  band_pass.o filters.o -O2 -o band_pass_debug `pkg-config --libs opencv`
band_pass.o: band_pass.cpp
	g++ -c -std=c++11 `pkg-config --cflags opencv` band_pass.cpp -O2 
filters.o: filters.cpp filters.hpp
	g++ -c `pkg-config --cflags opencv` filters.cpp -O2 
frame_colors:	
	g++ `pkg-config --cflags opencv` frame_colors.cpp -O2 -o frame_colors `pkg-config --libs opencv`
