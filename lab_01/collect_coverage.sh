#!/bin/bash

# Компиляция кода программы с необходимыми ключами
gcc -std=c99 -Wall -Werror -O0 -fprofile-arcs -ftest-coverage *.c -o app.exe -lm

# Запуск всех тестов
./func_tests/scripts/func_tests.sh

# Вывод информации о покрытии
gcov *.gcda

# Вывод дополнительной информации при аргументе -v
if [[ $1 == "-v" ]]; then
    lcov --capture --directory . --output-file coverage.info
    cat coverage.info
fi

# Очистка дополнительных файлов
./clean.sh

# Код завершения
exit 0
