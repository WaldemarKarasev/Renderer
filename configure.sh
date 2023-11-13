#! /bin/sh

cd build/
cmake -DGLFW_BUILD_DOCS=OFF -DSPDLOG_BUILD_SHARED=ON -S ../
cd ../