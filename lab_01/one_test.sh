#!/bin/bash

### Скрипт для запуска одного случая

./build_release.sh
code_result_build=$?

if [[ $code_result_build == 0 ]]; then
    ./app.exe
    code_result=$?
    echo "Код возврата: $code_result"
    ./clean.sh
else
    exit 1
fi

exit 0
