
# Define compiler and flags.
CPP       = g++ -ansi -g -O0
CFLAGS    = -c -I ./cppunit/include/
LINKFLAGS =  -o

# Define targets.
CPPFILES  = $(wildcard *.cpp)
HFILES    = $(CPPFILES:.cpp=.h)
OBJ       = $(CPPFILES:.cpp=.o)
LIBS      = ../src/specswap.a

# Use this for the local cppunit.
#LIBS2     = -L./cppunit/lib -l cppunit
# Use this for system wide cppunit.
LIBS2     = -l cppunit

# Make and run the tests.
all: tests run

.PHONY : run

run:
	./run_tests.x

# Tests target.
tests: $(OBJ) $(LIBS)
	$(CPP) $(LINKFLAGS) run_tests.x  $^ ${LIBS2}

# Rule for compiling.
%.o: %.cpp  $(patsubst %.o,%.h,$@)
	$(CPP) -o $@ $(CFLAGS) $<

# Rule for compiling the testRunner.
testRunner.o: testRunner.cpp $(HFILES)
	$(CPP) -o $@ $(CFLAGS) $<

# Make sure the library is allways up to date.
.PHONY : ../src/specswap.a

../src/specswap.a:
	cd ../src ; \
	rm specswap.a ; \
	make specswap.a ; \
	cd -

clean:
	rm *.o *.x *~
