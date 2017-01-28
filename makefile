# Helper makefile for libhexapod

build:
	mkdir -p build && cd build && cmake .. && make

test: build
	build/hex-test

util: build
	build/hex-util && ./graph.py

docs:
	doxygen ./doxygen.conf

clean:
	rm -rf build/

.PHONY: build test util clean
