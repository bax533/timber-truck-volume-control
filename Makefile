CC = g++
CFLAGS = -g -Wall
SRCS = main.cpp VolumeCalculator.hpp VolumeCalculator.cpp \
	   ImageHandler.hpp ImageHandler.cpp \
	   Common.hpp Tester.hpp Profiler.hpp \
	   ReferenceFinder.hpp ReferenceFinder.cpp

PROG = VolumeControl

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
