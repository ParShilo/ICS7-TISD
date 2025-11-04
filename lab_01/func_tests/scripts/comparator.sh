#!/bin/bash

# Ошибка при вводе не 2 или 3 аргументов
if [[ "$#" -lt 2 || "$#" -gt 3 ]]; then
    # Сообщение при ключе
    if [[ $3 == "-v" ]]; then
        echo "Error: should be only 2 or 3 args"
    fi
    exit 2
fi

# Проверка файлов на сущность файла
if [[ ! -f "$1" ]]; then
    if [[ $3 == "-v" ]]; then
        echo "Error: not found arg_1"
    fi
    exit 2
fi

if [[ ! -f "$2" ]]; then
    if [[ $3 == "-v" ]]; then
        echo "Error: not found arg_2"
    fi
    exit 2
fi

# Определение текста внутри файлов
file_1=$(cat "$1")
file_2=$(cat "$2")

# Строки с числами
numbers_1=""
numbers_2=""

# Счётчики чисел
count_1=0
count_2=0

# Выделение последовательностей чисел с помощью команды grep в цикле
for i in $file_1; do
    if echo "$i" | grep -Eq "^>>[+-]?([0-9]+([.][0-9]+)?|[0-9]*[.][0-9]+)([eE][+-]?[0-9]+)?$"; then
    count_1=$((count_1 + 1))
    numbers_1="$numbers_1 $i"
    fi
done

for j in $file_2; do
    if echo "$j" | grep -Eq "^>>[+-]?([0-9]+([.][0-9]+)?|[0-9]*[.][0-9]+)([eE][+-]?[0-9]+)?$"; then
    count_2=$((count_2 + 1))
    numbers_2="$numbers_2 $j"
    fi
done


#echo "$numbers_1"
#echo "$numbers_2"

# Проверка на равенство количества чисел
if [[ "$count_1" -ne "$count_2" ]]; then
    if [[ $3 == "-v" ]]; then
        echo "Not equal: Different length"
    fi
    exit 1
fi

# Сравнение последовательностей чисел
if [[ "$numbers_1" == "$numbers_2" ]]; then
    if [[ $3 == "-v" ]]; then
        echo "Equal"
    fi
    exit 0
else
    if [[ $3 == "-v" ]]; then
        echo "Not equal: Different numbers"
    fi
    exit 1
fi