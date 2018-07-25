# The Art of C++
# Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
# Please see LICENSE for license or visit https://github.com/taocpp/PEGTL

.SUFFIXES:
.SECONDARY:

ifeq ($(OS),Windows_NT)
UNAME_S := $(OS)
ifeq ($(shell gcc -dumpmachine),mingw32)
MINGW_CXXFLAGS = -U__STRICT_ANSI__
endif
else
UNAME_S := $(shell uname -s)
endif

# For Darwin (Mac OS X / macOS) we assume that the default compiler
# clang++ is used; when $(CXX) is some version of g++, then
# $(CXXSTD) has to be set to -std=c++11 (or newer) so
# that -stdlib=libc++ is not automatically added.

ifeq ($(CXXSTD),)
CXXSTD := -std=c++11
ifeq ($(UNAME_S),Darwin)
CXXSTD += -stdlib=libc++
endif
endif

# Ensure strict standard compliance and no warnings, can be
# changed if desired.

CPPFLAGS ?= -pedantic
CXXFLAGS ?= -Wall -Wextra -Wshadow -Werror -O3 $(MINGW_CXXFLAGS)

CLANG_TIDY ?= clang-tidy

HEADERS := $(filter-out include/tao/pegtl/internal/endian_win.hpp include/tao/pegtl/internal/file_mapper_win32.hpp,$(shell find include -name '*.hpp')) $(filter-out src/test/pegtl/main.hpp,$(shell find src -name '*.hpp'))
SOURCES := $(shell find src -name '*.cpp')
DEPENDS := $(SOURCES:%.cpp=build/%.d)
BINARIES := $(SOURCES:%.cpp=build/%)

UNIT_TESTS := $(filter build/src/test/%,$(BINARIES))

.PHONY: all
all: compile check

.PHONY: compile
compile: $(BINARIES)

.PHONY: check
check: $(UNIT_TESTS)
	@set -e; for T in $(UNIT_TESTS); do echo $$T; $$T > /dev/null; done

build/%.valgrind: build/%
	valgrind --error-exitcode=1 --leak-check=full $<
	@touch $@

.PHONY: valgrind
valgrind: $(UNIT_TESTS:%=%.valgrind)
	@echo "All $(words $(UNIT_TESTS)) valgrind tests passed."

build/%.cppcheck: %.hpp
	cppcheck --error-exitcode=1 --inconclusive --force --std=c++11 $<
	@mkdir -p $(@D)
	@touch $@

.PHONY: cppcheck
cppcheck: $(HEADERS:%.hpp=build/%.cppcheck)
	@echo "All $(words $(HEADERS)) cppcheck tests passed."

build/%.clang-tidy: %
	$(CLANG_TIDY) -extra-arg "-Iinclude" -extra-arg "-std=c++11" -checks=*,-fuchsia-*,-google-runtime-references,-google-runtime-int,-google-readability-todo,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-hicpp-no-array-decay,-hicpp-signed-bitwise,-modernize-raw-string-literal,-misc-sizeof-expression -warnings-as-errors=* $< 2>/dev/null
	@mkdir -p $(@D)
	@touch $@

.PHONY: clang-tidy
clang-tidy: $(HEADERS:%=build/%.clang-tidy) $(SOURCES:%=build/%.clang-tidy)
	@echo "All $(words $(HEADERS) $(SOURCES)) clang-tidy tests passed."

.PHONY: clean
clean:
	@rm -rf build
	@find . -name '*~' -delete

build/%.d: %.cpp Makefile
	@mkdir -p $(@D)
	$(CXX) $(CXXSTD) -Iinclude $(CPPFLAGS) -MM -MQ $@ $< -o $@

build/%: %.cpp build/%.d
	$(CXX) $(CXXSTD) -Iinclude $(CPPFLAGS) $(CXXFLAGS) $< -o $@

ifeq ($(findstring $(MAKECMDGOALS),clean),)
-include $(DEPENDS)
endif
