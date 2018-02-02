#GNU make

CXX=g++
CXXFLAGS=-std=c++17
VPATH=header:$(wildcard header/*):src:$(wildcard src/*)

obj=CSignal_guard.o CScopeGuard.o CSpin_mutex.o

all:$(obj)

.PHONY:clean
clean:
	-rm -f $(obj)

CSignal_guard.o:CSemaphore.hpp
