# Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
# Please see LICENSE for license or visit https://github.com/ColinH/PEGTL

ifeq ($(OS),Windows_NT)
UNAME_S := $(OS)
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
PEGTL_CXXFLAGS ?= -Wall -Wextra -Werror -O3
ifneq ($(filter coverage,$(MAKECMDGOALS)),)
PEGTL_CXXFLAGS := $(subst -O3,-O0 --coverage,$(PEGTL_CXXFLAGS))
endif

.PHONY: all coverage clean

SOURCES := $(wildcard */*.cc)
DEPENDS := $(SOURCES:%.cc=build/%.d)
BINARIES := $(SOURCES:%.cc=build/%)

UNIT_TESTS := $(filter build/unit_tests/%,$(BINARIES))

all: $(BINARIES)
	@echo "Built with '$(CXX) $(PEGTL_CXXSTD) -I. $(PEGTL_CPPFLAGS) $(PEGTL_CXXFLAGS)'."
	@set -e; for T in $(UNIT_TESTS); do echo $$T; $$T; done
	@echo "All $(words $(UNIT_TESTS)) unit tests passed."

# NOTE: Coverage analysis is considered experimental!

coverage: all
	@lcov -q -c -d . -b . -o build/gcov.info
	@lcov -q -r build/gcov.info "/usr*" -o build/gcov.info
	@genhtml -q build/gcov.info --demangle-cpp -o build/gcov/

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
