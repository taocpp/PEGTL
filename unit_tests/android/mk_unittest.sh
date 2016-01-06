#!/bin/bash

if [ $# -ne 1 ]
then
    echo "usage: $0 unittest"
    exit -1
fi

unittest="$1"

# remove old unit test
rm -rf jni/unittest.cc

# remove references to main.cc and test.hh
# rewrite file path
# add path as a parameter
cat "../$unittest.cc" \
  | sed -e 's/#include "main.hh"//g'  -e 's/#include "test.hh"//g'   \
  | sed -E 's/"unit_tests\/data\/(.*)\.json"/inPath+"\/\1.json"/g'   \
  | sed -e 's/void unit_test()/void unit_test\(std::string inPath\)/g' \
  > "jni/unittest.cc"
