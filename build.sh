#!/bin/bash

mkdir -p build
cd build
cmake .. && cmake --build . && cmake --install .