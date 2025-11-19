#!/bin/sh

#ctags ./src/*.cpp ./src/**/*.c ./include/*.hpp
find . \( -path "./src/*" -a \( -name "*.c" -o -name "*.cpp" \) \) -o \( -path "./include/*" -a \( -name "*.hpp" -o -name "*.h" \) \) | xargs ctags
