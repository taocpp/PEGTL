# Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
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
# $(PEGTL_CXXSTD) has to be set to -std=c++11 (or newer) so
# that -stdlib=libc++ is not automatically added.

ifeq ($(PEGTL_CXXSTD),)
PEGTL_CXXSTD := -std=c++11
ifeq ($(UNAME_S),Darwin)
PEGTL_CXXSTD += -stdlib=libc++
endif
endif

# Ensure strict standard compliance and no warnings, can be
# changed if desired.

PEGTL_CPPFLAGS ?= -pedantic
PEGTL_CXXFLAGS ?= -Wall -Wextra -Werror -Wshadow -O3 $(MINGW_CXXFLAGS)

.PHONY: all compile check valgrind cppcheck clean

HEADERS := pegtl.hh $(shell find pegtl -name '*.hh')
SOURCES := $(wildcard */*.cc)
DEPENDS := $(SOURCES:%.cc=build/%.d)
BINARIES := $(SOURCES:%.cc=build/%)

UNIT_TESTS := $(filter build/unit_tests/%,$(BINARIES))

all: compile check

compile: $(BINARIES)

check: $(UNIT_TESTS)
	@echo "Built with '$(CXX) $(PEGTL_CXXSTD) -I. $(PEGTL_CPPFLAGS) $(PEGTL_CXXFLAGS)'."
	@set -e; for T in $(UNIT_TESTS); do echo $$T; $$T < unit_tests/file_data.txt > /dev/null; done
	@echo "All $(words $(UNIT_TESTS)) unit tests passed."

build/%.valgrind: build/%
	valgrind --error-exitcode=1 --leak-check=full $< <unit_tests/file_data.txt
	@touch $@

valgrind: $(UNIT_TESTS:%=%.valgrind)
	@echo "All $(words $(UNIT_TESTS)) valgrind tests passed."

build/%.cppcheck: %.hh
	@mkdir -p $(@D)
	cppcheck --error-exitcode=1 --enable=warning --inconclusive --force --std=c++11 $<
	@touch $@

cppcheck: $(HEADERS:%.hh=build/%.cppcheck)
	@echo "All $(words $(HEADERS)) cppcheck tests passed."

clean:
	rm -rf build/*

.SECONDARY:

build/%.d: %.cc Makefile
	@mkdir -p $(@D)
	$(CXX) $(PEGTL_CXXSTD) -I. $(PEGTL_CPPFLAGS) -MM -MQ $@ $< -o $@

build/%: %.cc build/%.d
	$(CXX) $(PEGTL_CXXSTD) -I. $(PEGTL_CPPFLAGS) $(PEGTL_CXXFLAGS) $< -o $@

ifeq ($(findstring $(MAKECMDGOALS),clean),)
-include $(DEPENDS)
endif
