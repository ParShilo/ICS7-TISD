#!/bin/bash

gcc -c -g3 -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla *.c

gcc -o app.exe *.o -lm
