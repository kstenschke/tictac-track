#!/bin/bash

# Generate Makefile from CMakeLists.txt
cmake CMakeLists.txt

# Compile, link
make

# Move into bin/<CURRENT_OS> directory
case "$OSTYPE" in
  bsd*)     OS="bsd" ;;
  darwin*)  OS="osx" ;;
  linux*)   OS="linux" ;;
  solaris*) OS="solaris" ;;
  *)        OS="unknown: $OSTYPE" ;;
esac

if [ "$OS" == "osx" ]; then
  mv ./tsp bin/mac/tsp
elif [ $OS == "linux" ]; then
  mv ./tsp bin/linux/tsp
fi
