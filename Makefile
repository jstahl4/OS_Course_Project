#
# Makefile for the disk emulator
# Author: D.M. Lyons
# Date: Spring 2017
#
# A List of all the source files that should be compiled together
#
SOURCES = diskprocess.cpp demo.cpp
#
# A list of the objects that result from the compilation process
#
OBJECTS = diskprocess.o demo.o
#
# A list of include files on which theh compilation depends
#
DEFS = diskprocess.h
#
# What flags to send to the compiler when you compile source files
#
CPPFLAGS = -g
#
# Any libraries that should be included in the linkage step of compilation
LIBFLAGS = -lm

#

demo: $(SOURCES) $(DEFS)
	g++ $(CPPFLAGS) $(SOURCES) -o demo $(LIBFLAGS)
