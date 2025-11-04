#!/bin/bash

# Путь к временному файлу
actual_output="./temp_output.txt"

# Определение потоков ввода
input_file="$1"

# Создание временного файла
touch "$actual_output"

# Выполнение программы
./../../app.exe <"$input_file" >"$actual_output"

# Сохранение кода завершения
result=$?

#cat "$actual_output"

# Выход с кодом возврата от результата работы программы
if [[ $result != 0 ]]; then
    exit 0
else
    exit 1
fi
