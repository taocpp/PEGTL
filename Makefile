# Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
# Please see LICENSE for license or visit https://github.com/ColinH/PEGTL

ifeq ($(OS),Windows_NT)
UNAME_S := $(OS)
ifeq ($(shell gcc -dumpmachine),mingw32)
MINGW_CXXFLAGS = -U__STRICT_ANSI__
endif
else
UNAME_S := $(shell uname -s)
endif

# For Darwin (Mac OS X) we assume that the default compiler
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

.PHONY: all compile check valgrind cppcheck clang-tidy clean

HEADERS := pegtl.hh $(shell find pegtl -name '*.hh')
SOURCES := $(wildcard */*.cc)
DEPENDS := $(SOURCES:%.cc=build/%.d)
BINARIES := $(SOURCES:%.cc=build/%)

UNIT_TESTS := $(filter build/unit_tests/%,$(BINARIES))

all: compile check

compile: $(BINARIES)

check: $(UNIT_TESTS)
	@set -e; for T in $(UNIT_TESTS); do echo $$T; $$T > /dev/null; done
	@echo "All $(words $(UNIT_TESTS)) unit tests passed."

build/%.valgrind: build/%
	valgrind --error-exitcode=1 --leak-check=full $<
	@touch $@

valgrind: $(UNIT_TESTS:%=%.valgrind)
	@echo "All $(words $(UNIT_TESTS)) valgrind tests passed."

build/%.cppcheck: %.hh
	cppcheck --error-exitcode=1 --inconclusive --force --std=c++11 $<
	@mkdir -p $(@D)
	@touch $@

cppcheck: $(HEADERS:%.hh=build/%.cppcheck)
	@echo "All $(words $(HEADERS)) cppcheck tests passed."

build/%.clang-tidy: %
	$(CLANG_TIDY) -extra-arg "-I." -extra-arg "-std=c++11" -checks=*,-google-*,-llvm-include-order,-clang-analyzer-alpha*,-cppcoreguidelines*,-readability-named-parameter $< 2>/dev/null
	@mkdir -p $(@D)
	@touch $@

clang-tidy: $(HEADERS:%=build/%.clang-tidy) $(SOURCES:%=build/%.clang-tidy)
	@echo "All $(words $(HEADERS) $(SOURCES)) clang-tidy tests passed."

clean:
	rm -rf build/*

.SECONDARY:

build/%.d: %.cc Makefile
	@mkdir -p $(@D)
	$(CXX) $(CXXSTD) -I. $(CPPFLAGS) -MM -MQ $@ $< -o $@

build/%: %.cc build/%.d
	$(CXX) $(CXXSTD) -I. $(CPPFLAGS) $(CXXFLAGS) $< -o $@

ifeq ($(findstring $(MAKECMDGOALS),clean),)
-include $(DEPENDS)
endif
