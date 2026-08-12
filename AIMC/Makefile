CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Wextra

EXES = std.exe brute.exe validator.exe gen_random.exe gen_extreme.exe stress.exe

.PHONY: all clean tests check

all: $(EXES)

std.exe: std.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

brute.exe: brute.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

validator.exe: validator.cpp testlib.h
	$(CXX) $(CXXFLAGS) -o $@ $<

gen_random.exe: gen_random.cpp testlib.h
	$(CXX) $(CXXFLAGS) -o $@ $<

gen_extreme.exe: gen_extreme.cpp testlib.h
	$(CXX) $(CXXFLAGS) -o $@ $<

stress.exe: stress.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

tests: all
	bash run.sh

check: stress.exe std.exe brute.exe
	./stress.exe 100 1

clean:
	rm -f $(EXES) _stress.in _std.out _brute.out _g.in _g.ans _g.err
