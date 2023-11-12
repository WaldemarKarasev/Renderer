#! /bin/sh

cd build/
cmake -DGLFW_BUILD_DOCS=OFF -S ../
cd ../