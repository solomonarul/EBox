# This Makefile is meant to be used only in development environments.

.DEFAULT_GOAL := _bruh

TARGET = EBox

_bruh:
	@echo "You didn't read the README, did you?"

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

pp:
	@awk -F '|' -f ./scripts/bf_parse_performance_log.awk ./performance.log | sort -k1,1nr

cov:
	@gcov ./build/CMakeFiles/EBox_tests.dir/test/main.c.gcda