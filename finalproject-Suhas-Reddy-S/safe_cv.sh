#!/bin/bash

SOURCE_FILE="safe_cv.cpp"
EXECUTABLE="safe_cv"

case $1 in
    build) g++ -std=c++11 -o $EXECUTABLE $SOURCE_FILE -pthread && echo "Build successful." ;;
    clean) rm -f $EXECUTABLE && echo "Cleaned." ;;
    run) ./$EXECUTABLE || echo "Executable not found, build first." ;;
    *) echo "Usage: $0 {build|clean|run}" ;;
esac
