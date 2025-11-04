#!/bin/bash

clang -std=c99 -Wall -Werror -fsanitize=undefined -fno-omit-frame-pointer -g *.c -lm -o app.exe
