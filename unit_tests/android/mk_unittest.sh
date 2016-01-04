#!/bin/bash

if [ $# -ne 1 ]
then
    echo "usage: $0 unittest"
    exit -1
fi

unittest="$1"

# remove old unit test
rm -rf jni/unittest.cc

# copy and remove reference to main.hh
cat "../$unittest.cc" | sed -e 's/#include "main.hh"//g' > "jni/unittest.cc" -e 's/#include "test.hh"//g'
