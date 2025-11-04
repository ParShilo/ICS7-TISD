#!/bin/bash

clang -std=c99 -Wall -Werror -fsanitize=memory -fPIE -pie -fno-omit-frame-pointer -g *.c -lm -o app.exe
