#!/bin/bash

# Путь, где находится скрипт
script_dir=$(cd "$(dirname "$0")" && pwd)

# Переход к директории скрипта
cd "$script_dir" || exit 1

# Директория с тестами
tests_dir="../data"

# Путь к временному файлу
actual_output="./temp_output.txt"

# Считывание количества тестовых файлов
pos_in_count=$(find "$tests_dir" -maxdepth 1 -name 'pos_*_in.txt' | wc -l)
pos_out_count=$(find "$tests_dir" -maxdepth 1 -name 'pos_*_out.txt' | wc -l)

# Проверка на наличие скрипта и тестовых файлов
if [[ -f "./pos_case.sh" && $pos_in_count -gt 0 && $pos_out_count -gt 0 ]]; then
    pos_exist=1
else
    pos_exist=0
fi

# Количество пройденных тестов и не пройденных
pos_passed_tests=0
pos_failed_tests=0

if [[ $pos_exist == 1 ]]; then

    # Поиск файлов с входными данными
    for pos_input_file in "$tests_dir"/pos_*_in.txt; do

        # Извлечение номера теста из имени файла
        pos_test_num=$(echo "$pos_input_file" | sed -E 's/.*pos_([0-9]+)_in\.txt/\1/')

        # Соответствующий файл с ожидаемым выводом
        expected_file="$tests_dir/pos_${pos_test_num}_out.txt"

        # Проверка файла с ожидаемым результатом
        if [[ -f $expected_file ]]; then

            # Запуск скрипта для каждого случая и запоминание результата
            ./pos_case.sh "$pos_input_file" "$expected_file"
            pos_result=$?

            # Удаление временного файла
            rm -f "$actual_output"

            # Проверка кода возврата
            if [[ $pos_result == 0 ]]; then
                if [[ $1 == "-v" ]]; then
                    echo -e "Test pos_${pos_test_num}: \033[92mpassed\033[0m"
                fi
                pos_passed_tests=$((pos_passed_tests + 1))
            else
                if [[ $1 == "-v" ]]; then
                    echo -e "Test pos_${pos_test_num}: \033[91mNOT passed\033[0m"
                fi
                pos_failed_tests=$((pos_failed_tests + 1))
            fi
        else
            if [[ $1 == "-v" ]]; then
                echo "Error: not found file with expected output for test pos_${pos_test_num}"
            fi
        fi
    done

    # Вывод количества успешных тестов
    if [[ $pos_failed_tests == 0 ]]; then
        echo -e "~~~ \033[92mPassed ${pos_passed_tests} / $((pos_passed_tests + pos_failed_tests)) pos_tests\033[0m ~~~"
    else
        echo -e "~~~ \033[91mPassed ${pos_passed_tests} / $((pos_passed_tests + pos_failed_tests)) pos_tests\033[0m ~~~"
    fi
fi

### --- Теперь негативные тесты --- ###

# Считывание количества тестовых файлов
neg_in_count=$(find "$tests_dir" -maxdepth 1 -name 'neg_*_in.txt' | wc -l)
neg_out_count=$(find "$tests_dir" -maxdepth 1 -name 'neg_*_out.txt' | wc -l)

# Проверка на наличие скрипта и тестовых файлов
if [[ -f "neg_case.sh" && $neg_in_count -gt 0 && $neg_out_count -gt 0 ]]; then
    neg_exist=1
else
    neg_exist=0
fi

# Количество пройденных тестов и не пройденных
neg_passed_tests=0
neg_failed_tests=0

if [[ $neg_exist == 1 ]]; then

    # Поиск файлов с входными данными
    for neg_input_file in "$tests_dir"/neg_*_in.txt; do

        # Извлечение номера теста из имени файла
        neg_test_num=$(echo "$neg_input_file" | sed -E 's/.*neg_([0-9]+)_in\.txt/\1/')

        # Запуск скрипта для каждого случая и запоминание результата
        ./neg_case.sh "$neg_input_file"
        neg_result=$?

        # Удаление временного файла
        rm -f "$actual_output"

        # Проверка кода завершения
        if [[ $neg_result == 0 ]]; then
            if [[ $1 == "-v" ]]; then
                echo -e "Test neg_${neg_test_num}: \033[92mpassed\033[0m"
            fi
            neg_passed_tests=$((neg_passed_tests + 1))
        else
            if [[ $1 == "-v" ]]; then
                echo -e "Test neg_${neg_test_num}: \033[91mNOT passed\033[0m"
            fi
            neg_failed_tests=$((neg_failed_tests + 1))
        fi
    done

    # Вывод количества успешных тестов
    if [[ $neg_failed_tests == 0 ]]; then
        echo -e "~~~ \033[92mPassed ${neg_passed_tests} / $((neg_passed_tests + neg_failed_tests)) neg_tests\033[0m ~~~"
    else
        echo -e "~~~ \033[91mPassed ${neg_passed_tests} / $((neg_passed_tests + neg_failed_tests)) neg_tests\033[0m ~~~"
    fi
fi

# Сообщение при отсутствии тестов или скриптов
if [[ $pos_exist == 0 && $pos_exist == 0 ]]; then
    echo -e "\033[1;93mNo tests or scripts\033[0m"
fi

# Если все тесты прошли, завершение с кодом 0
exit $((pos_failed_tests + neg_failed_tests))
