#!/bin/bash

# Путь, где находится скрипт
script_dir=$(cd "$(dirname "$0")" && pwd)

# Переход к директории скрипта и выход в случае
cd "$script_dir" || exit 1

# Начальная очистка
./clean.sh

./build_debug_asan.sh
code_result=$?
echo "1) BUILD_DEBUG_ASAN:"

if [[ $code_result == 0 ]]; then
    if [[ $1 == "-v" ]]; then
        ./func_tests/scripts/func_tests.sh -v
    else
        ./func_tests/scripts/func_tests.sh
    fi
else
    if [[ $1 == "-v" ]]; then
        echo "!!! Error: Build_Debug_Asan"
    fi
fi

./clean.sh

./build_debug_msan.sh
code_result=$?
echo "2) BUILD_DEBUG_MSAN:"

if [[ $code_result == 0 ]]; then
    if [[ $1 == "-v" ]]; then
        ./func_tests/scripts/func_tests.sh -v
    else
        ./func_tests/scripts/func_tests.sh
    fi
else
    if [[ $1 == "-v" ]]; then
        echo "!!! Error: Build_Debug_Msan"
    fi
fi

./clean.sh

./build_debug_ubsan.sh
code_result=$?
echo "3) BUILD_DEBUG_UBSAN:"

if [[ $code_result == 0 ]]; then
    if [[ $1 == "-v" ]]; then
        ./func_tests/scripts/func_tests.sh -v
    else
        ./func_tests/scripts/func_tests.sh
    fi
else
    if [[ $1 == "-v" ]]; then
        echo "!!! Error: Build_Debug_Ubsan"
    fi
fi

./clean.sh

exit 0
