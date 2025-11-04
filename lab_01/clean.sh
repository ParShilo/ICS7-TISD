#!/bin/bash

# Команда для удаления всех файлов кроме файлов с данными расширениями
find . -type f ! -name "*.c" ! -name "*.sh" ! -name "*.txt" ! -name "*.md" ! -name "*.h" -delete

# Сообщение о завершении работы скрипта при ключе
if [[ $3 == "-v" ]]; then
    echo "Cleaning succeeded."
fi

exit 0
