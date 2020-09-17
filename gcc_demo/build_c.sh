#!/bin/sh

#针对小程序，使用这样的编译方法就行了

gcc $@ -o build_result -lm
./build_result
rm -rf build_result