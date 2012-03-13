all:
	g++ -bind_at_load `pkg-config --cflags opencv` machine_learning.cpp -o machine_learning `pkg-config --libs opencv`
debug:
	g++ -bind_at_load `pkg-config --cflags opencv` machine_learning.cpp -g -o machine_learning `pkg-config --libs opencv`
