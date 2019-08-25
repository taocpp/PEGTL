# The Art of C++
# Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
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
CXXFLAGS ?= -Wall -Wextra -Wshadow -Wconversion -Werror -O3 $(MINGW_CXXFLAGS)

CLANG_TIDY ?= clang-tidy

HEADERS := $(shell find include -name '*.hpp')
SOURCES := $(shell find src -name '*.cpp')
DEPENDS := $(SOURCES:%.cpp=build/%.d)
BINARIES := $(SOURCES:%.cpp=build/%)

CLANG_TIDY_HEADERS := $(filter-out include/tao/pegtl/internal/endian_win.hpp include/tao/pegtl/internal/file_mapper_win32.hpp,$(HEADER)) $(filter-out src/test/pegtl/main.hpp,$(shell find src -name '*.hpp'))

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

build/%.clang-tidy: %
	$(CLANG_TIDY) -extra-arg "-Iinclude" -extra-arg "-std=c++11" -checks=*,-fuchsia-*,-google-runtime-references,-google-runtime-int,-google-readability-todo,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-*-magic-numbers,-cppcoreguidelines-non-private-member-variables-in-classes,-cppcoreguidelines-macro-usage,-hicpp-no-array-decay,-hicpp-signed-bitwise,-modernize-raw-string-literal,-misc-sizeof-expression,-misc-non-private-member-variables-in-classes,-bugprone-sizeof-expression,-bugprone-exception-escape -warnings-as-errors=* $< 2>/dev/null
	@mkdir -p $(@D)
	@touch $@

.PHONY: clang-tidy
clang-tidy: $(CLANG_TIDY_HEADERS:%=build/%.clang-tidy) $(SOURCES:%=build/%.clang-tidy)
	@echo "All $(words $(CLANG_TIDY_HEADERS) $(SOURCES)) clang-tidy tests passed."

.PHONY: clean
clean:
	@rm -rf build
	@find . -name '*~' -delete

build/%.d: %.cpp Makefile
	@mkdir -p $(@D)
	$(CXX) $(CXXSTD) -Iinclude $(CPPFLAGS) -MM -MQ $@ $< -o $@

build/%: %.cpp build/%.d
	$(CXX) $(CXXSTD) -Iinclude $(CPPFLAGS) $(CXXFLAGS) $< -o $@

.PHONY: amalgamate
amalgamate: build/amalgamated/pegtl.hpp

build/amalgamated/pegtl.hpp: $(HEADERS)
	@mkdir -p $(@D)
	@rm -rf build/include
	@cp -a include build/
	@rm -rf build/include/tao/pegtl/contrib/icu
	@rm -rf build/include/tao/pegtl/contrib/changes.hpp
	@sed -i -e 's%^#%//#%g' $$(find build/include -name '*.hpp')
	@sed -i -e 's%^//#include "%#include "%g' $$(find build/include -name '*.hpp')
	@for i in $$(find build/include -name '*.hpp'); do echo "#pragma once" >tmp.out; echo "#line 1" >>tmp.out; cat $$i >>tmp.out; mv tmp.out $$i; done
	@echo '#include "tao/pegtl.hpp"' >build/include/amalgamated.hpp
	@echo '#include "tao/pegtl/analyze.hpp"' >>build/include/amalgamated.hpp
	@( cd build/include ; for i in tao/pegtl/contrib/*.hpp; do echo "#include \"$$i\""; done ) >>build/include/amalgamated.hpp
	@echo -e "/*\n\nWelcome to the Parsing Expression Grammar Template Library (PEGTL)." >$@
	@echo -e "See https://github.com/taocpp/PEGTL/ for more information, documentation, etc.\n" >>$@
	@echo -e "The library is licensed as follows:\n" >>$@
	@cat LICENSE >>$@
	@echo -e "\n*/\n" >>$@
	@( cd build/include ; g++ -E -C -nostdinc amalgamated.hpp ) >>$@
	@sed -i -e 's%^//#%#%g' $@
	@sed -i -e 's%^# \([0-9]* "[^"]*"\).*%#line \1%g' $@
	@sed -i -e 's%^// Copyright.*%%g' $@
	@sed -i -e 's%^// Please.*%%g' $@
	@echo "Generated/updated $@ successfully."

ifeq ($(findstring $(MAKECMDGOALS),clean),)
-include $(DEPENDS)
endif
