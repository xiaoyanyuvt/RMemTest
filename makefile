all:foo
foo: foo.cpp
	g++ -g -std=c++11 foo.cpp -o foo -ldl
clean: 
	rm foo
