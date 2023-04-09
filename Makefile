
GCC = g++

DEPS = src/library/common/ src/library/
SOURCES = src/library/common/queue.hpp src/library/common/worker.hpp src/library/pipeline.hpp src/library/farm.hpp

BUILDARGS = $(foreach d, $(DEPS), -I$d) -g

example: examples/example.c
	$(GCC) examples/example.c -o builds/example -g

convolution: examples/convolution.cpp
	$(GCC) examples/convolution.cpp -o builds/convolution -lpng -g

test_fib: $(SOURCES)
	$(GCC) src/test_fib.cpp $(BUILDARGS) -o builds/test_fib

test_conv: $(SOURCES)
	$(GCC) src/test_convolution.cpp $(BUILDARGS) -lpng -o builds/test_conv

test: test_fib test_conv

all: example convolution test

clean:
	rm -rf builds/*