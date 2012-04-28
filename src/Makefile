# Makefile for SpecSwap-RMC

# Define compilers.
CPP = g++ -ansi -pedantic
FORTRAN=gfortran
#FORTRAN=ifort

# Define compiler flags for debug.
#FLAGS = -Wall -finline-functions -funroll-loops -frerun-loop-opt -g -pg -O2 -c

# Flags for production runs.
FLAGS = -Wall -finline-functions -funroll-loops -frerun-loop-opt -O2 -c

FOPT0= -g -pg -O2 -c
#FOPT0= -O2 -c

# Define linking flags.
LINK = -g -pg -Wall -o
#LINK = -Wall -o

# Used for mklib.
MKLIBMAIN=mklibmain.o
MKLIBOBJCPP=mklib.o library.o matrix.o ioutils.o ioexception.o mathutils.o
MKLIBINCL=
MKLIBLIBS=

SPECSWAPMAIN=specswapmain.o
SPECSWAPOBJCPP=mlrmc.o specswap.o library.o sampleset.o mlrnd.o mathutils.o ioutils.o ioexception.o matrix.o pcfdata.o curvedata.o meanscalardata.o valuescalardata.o scalardistributiondata.o
SPECSWAPOBJF=randf.o
SPECSWAPOBJC=
SPECSWAPLIBS=

TARGET=SpecSwapRMC_1.0a0.x

# Target specswap.
specswap: $(SPECSWAPMAIN) $(SPECSWAPOBJCPP) $(SPECSWAPOBJF) $(SPECSWAPOBJC)
	$(CPP) $(LINK) $(TARGET) $(SPECSWAPMAIN) $(SPECSWAPOBJCPP) $(SPECSWAPOBJF) $(SPECSWAPOBJC) $(SPECSWAPLIBS)

# Target mklib.
mklib: $(MKLIBMAIN) $(MKLIBOBJCPP) $(MKLIBINCL)
	 $(CPP) $(LINK) mklib3.0.x $(MKLIBMAIN) $(MKLIBOBJCPP) $(MKLIBLIBS)

# Target for testing.
specswap.a:$(MKLIBOBJCPP) $(SPECSWAPOBJCPP) $(SPECSWAPOBJF) $(SPECSWAPOBJC)
	ar rvs specswap.a $(MKLIBOBJCPP) $(SPECSWAPOBJCPP) $(SPECSWAPOBJF) $(SPECSWAPOBJC)

# The main driver for the mklib program.
mklibmain.o:mklibmain.cpp mklib.h
	$(CPP) $(FLAGS) -o $@ $<

# The mklib object used in mklib.
mklib.o:mklib.cpp mklib.h library.h
	$(CPP) $(FLAGS) -o $@ $<

# The library object used in both mklib and specswap.
library.o: library.cpp library.h matrix.h ioutils.h mathutils.h
	$(CPP) $(FLAGS) -o $@ $<

# The main driver for the specswap program.
specswapmain.o:specswapmain.cpp mlrmc.h ioutils.h
	$(CPP) $(FLAGS) -o $@ $<

# The mlrmc class handling input reading and rmc run used in specswap.
mlrmc.o:mlrmc.cpp mlrmc.h mlrnd.h mlrmc.h mlrnd.h specswap.h ioutils.h sampleset.h
	$(CPP) $(FLAGS) -o $@ $<

# The specswap class used for handling specswap moves.
specswap.o:specswap.cpp specswap.h library.h sampleset.h mathutils.h ioutils.h pcfdata.h curvedata.h meanscalardata.h valuescalardata.h
	$(CPP) $(FLAGS) -o $@ $<

# The sampleset class used in specswap.
sampleset.o:sampleset.cpp sampleset.h mathutils.h ioutils.h
	$(CPP) $(FLAGS) -o $@ $<

# The random number generator.
mlrnd.o:mlrnd.cpp mlrnd.h randf.h
	$(CPP) $(FLAGS) -o $@ $<

# Our fortran random number generator.
randf.o:randf.f randf.h
	$(FORTRAN) $(FOPT0) -o randf.o randf.f

# The matrix class.
matrix.o: matrix.cpp matrix.h
	$(CPP) $(FLAGS) -o $@ $<

# The IO utilities.
ioutils.o: ioutils.cpp ioutils.h ioexception.h
	$(CPP) $(FLAGS) -o $@ $<

# The IO exception.
ioexception.o: ioexception.cpp ioexception.h
	$(CPP) $(FLAGS) -o $@ $<

# The math utilities class.
mathutils.o: mathutils.cpp mathutils.h matrix.h
	$(CPP) $(FLAGS) -o $@ $<

# The class for handling pcf data.
pcfdata.o: pcfdata.cpp pcfdata.h library.h mathutils.h ioutils.h
	$(CPP) $(FLAGS) -o $@ $<

# The class for handling curve data.
curvedata.o: curvedata.cpp curvedata.h library.h mathutils.h ioutils.h
	$(CPP) $(FLAGS) -o $@ $<

# The class for handling mean scalar data.
meanscalardata.o: meanscalardata.cpp meanscalardata.h library.h
	$(CPP) $(FLAGS) -o $@ $<

# The class for handling value scalar data.
valuescalardata.o: valuescalardata.cpp valuescalardata.h library.h
	$(CPP) $(FLAGS) -o $@ $<

# The class for handling scalar distribution data.
scalardistributiondata.o: scalardistributiondata.cpp scalardistributiondata.h library.h mathutils.h ioutils.h
	$(CPP) $(FLAGS) -o $@ $<

# delete all object files
clean:
	rm *.o *.x *.a
