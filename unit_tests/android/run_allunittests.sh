#!/bin/bash

for name in $(find .. -name '*.cc' -exec grep -H "void unit_test" {} \; | cut -d/ -f2 | cut -d. -f1)
do
    echo "Building and running $name"
    ./mk_unittest.sh $name
    ./build_unittest.sh
    ant installd
    ./run_unittest.sh
    echo "Done $name"
done
