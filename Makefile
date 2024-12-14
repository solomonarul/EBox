# This Makefile is meant to be used only in development environments.

.DEFAULT_GOAL := _bruh

TARGET = EBox

_bruh:
	@echo "You didn't read the README, did you?"

c:
	@cmake -E remove_directory build && echo "Removed build directory."
	@cmake -E remove_directory .cache && echo "Removed .cache directory."

bud:
	@cmake -B build -S . --preset unix-debug
	@cmake --build build

bur:
	@cmake -B build -S . --preset unix-release
	@cmake --build build

bwd:
	@cmake -B build -S . --preset windows-debug -DENABLE_JIT=OFF
	@cmake --build build

bwr:
	@cmake -B build -S . --preset windows-release -DENABLE_JIT=OFF
	@cmake --build build

bvd:
	@cmake -B build -S . --preset vita-debug
	@cmake --build build

bvr:
	@cmake -B build -S . --preset vita-release
	@cmake --build build

r:
	@./build/${TARGET} startup.ini
