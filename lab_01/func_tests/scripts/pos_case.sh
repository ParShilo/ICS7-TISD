#!/bin/bash

# Путь к временному файлу
actual_output="./temp_output.txt"

# Определение потоков ввода и ожидаемого вывода
input_file="$1"
expected_file="$2"

# Создание временного файла
touch "$actual_output"

# Выполнение программы
./../../app.exe <"$input_file" >"$actual_output"
result_code=$?

# Проверка кода возврата
if [[ $result_code != 0 ]]; then
    result=1
else
    # Запуск компаратора
    ./comparator.sh "$actual_output" "$expected_file"

    # Сохранение кода завершения
    result=$?
fi

# Выход с результатом работы
exit $result
