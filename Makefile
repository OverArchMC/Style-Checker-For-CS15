CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow -std=c++11

checker: main.o StyleChecker.o Function.o
	$(CXX) $(CXXFLAGS) $^ -o checker

StyleChecker.o: StyleChecker.cpp StyleChecker.h
	$(CXX) $(CXXFLAGS) -c StyleChecker.cpp

Function.o: Function.cpp Function.h
	$(CXX) $(CXXFLAGS) -c Function.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp


# remove executables, object code, and temporary files from the current folder 
# -- the executable created by unit_test is called a.out
clean:
	rm *.o *~ a.out
