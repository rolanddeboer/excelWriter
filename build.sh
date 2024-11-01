#!/bin/bash

mkdir -p build
cd build
cmake .. && cmake --build . && cmake --install .

sudo cp -r excelWriter /usr/local/bin