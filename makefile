files = main.cpp winFromFile.cpp featureNamespace.cpp randomStuff.cpp movement.cpp
main: $(files)
	g++ -o main $(files) -g -lncurses
