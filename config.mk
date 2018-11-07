#ASMTOOL := asminst -a x86_64_gcc -k --
CXX := ${ASMTOOL} g++
CC := ${ASMTOOL} gcc
LD := ${CXX}

CXXFLAGS := -fPIC -std=c++17 -Wall -Wextra
CFLAGS := -fPIC -std=c11 -Wall -Wextra
LDFLAGS := 
