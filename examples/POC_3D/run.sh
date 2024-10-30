#!/bin/bash

build_project() {
    mkdir -p build
    cd build
    touch .gitkeep

    cmake .. -DCMAKE_TOOLCHAIN_FILE=./cmake/define-compilers.cmake -DCMAKE_BUILD_TYPE=Debug || { echo "CMake configuration failed"; exit 1; }
    cmake --build . --parallel 8 || { echo "CMake build failed"; exit 1; }
}

cd ../../
build_project

cd ../examples/POC_3D
build_project

echo "Running server..."
mkfifo pipe
./r-type_serverd > pipe &
SERVER_PID=$!
tee server_output.log < pipe &
TEE_PID=$!

cleanup() {
    echo "\nCleaning up..."
    kill $SERVER_PID 2>/dev/null
    kill $TEE_PID 2>/dev/null
    rm pipe
    kill $CLIENT_PID 2>/dev/null
    exit 0
}

# Trap CTRL + C (SIGINT)
trap cleanup SIGINT

sleep 1

echo "Running client..."
./r-type_clientd

kill $SERVER_PID 2>/dev/null
kill $TEE_PID 2>/dev/null
rm pipe

cd ..
exit 0
