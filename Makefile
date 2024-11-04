# This Makefile is meant to be used only in development environments.

.DEFAULT_GOAL := r

TARGET = EBox

c:
	@cmake -E remove_directory build
	@cmake -E remove_directory .cache

bud:
	@cmake -B build -S . --preset unix-debug
	@cmake --build build

bur:
	@cmake -B build -S . --preset unix-release
	@cmake --build build

r:
	@./build/${TARGET} ./samples/bf/mandlebrot.b

t:
	@valgrind ./build/${TARGET}_tests

b:
	@./build/${TARGET}_bench