#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "err_def.h"
#include "num_func.h"

int parse_number(char *number_str, number *number, const size_t max_size_mantissa)
{
    number_str[strcspn(number_str, "\n")] = '\0';
    size_t len = strlen(number_str);

    if (len == 0)
        return ERROR_EMPTY;

    char status = 0; // 0-знак, 1-мантисса, 2-знак экспоненты, 3-экспонента
    char nools = 0, has_digit = 0;
    size_t len_mantissa = 0;
    int pos_dot = -1;

    number->exponent = 0;
    number->sign_mantissa = '+';
    number->sign_exponent = '+';

    for (size_t i = 0; i < len; i++)
    {
        // Считывание знака
        if (status == 0)
        {
            if (number_str[i] == '+')
            {
                status = 1;
                number->sign_mantissa = '+';
            }
            else if (number_str[i] == '-')
            {
                status = 1;
                number->sign_mantissa = '-';
            }
            else if (number_str[i] == '0')
            {
                status = 1;
                has_digit = 1;
                nools = 1;
                number->mantissa[len_mantissa++] = number_str[i];
            }
            else if (isdigit(number_str[i]))
            {
                status = 1;
                has_digit = 1;
                number->mantissa[len_mantissa++] = number_str[i];
            }
            else if (number_str[i] == '.') 
            {
                status = 1;
                nools = 0;
                pos_dot = 0;
            }
            else
                return ERROR_INPUT;
        }
        // Считывание мантиссы 
        else if (status == 1)
        {
            if (number_str[i] == 'E')
                status = 2;
            else if (isdigit(number_str[i]))
            {
                // Обработка нулей
                if (nools && number_str[i] != '0')
                {
                    nools = 0;
                    number->mantissa[len_mantissa++] = number_str[i];
                    if (len_mantissa > max_size_mantissa)
                        return ERROR_LONG_MANTISSA;
                }
                else if (!nools)
                {
                    number->mantissa[len_mantissa++] = number_str[i];
                    if (len_mantissa > max_size_mantissa)
                        return ERROR_LONG_MANTISSA;
                }
                else if (nools && number_str[i] == '0' && !has_digit)
                {
                    has_digit = 1;
                    number->mantissa[len_mantissa++] = number_str[i];
                }
            }
            else if (number_str[i] == '.')
            {
                if (pos_dot == -1)
                {
                    nools = 0;
                    pos_dot = len_mantissa;
                }
                else 
                    return ERROR_INPUT;
            }
            else
                return ERROR_INPUT;
        }
        // Считывание знака экспоненты
        else if (status == 2)
        {
            if (number_str[i] == '+')
            {
                status = 3;
                number->sign_exponent = '+';
            }
            else if (number_str[i] == '-')
            {
                status = 3;
                number->sign_exponent = '-';
            }
            else if (isdigit(number_str[i]))
            {
                status = 3;
                number->exponent = number_str[i] - '0';
            }
            else
                return ERROR_INPUT;
        }
        // Считывание экспоненты
        else if (status == 3)
        {
            if (isdigit(number_str[i]))
            {
                number->exponent = number->exponent * 10 + (number_str[i] - '0');
                if (number->exponent > 99999)
                    return ERROR_LONG_EXPONENTA;
            }
            else
                return ERROR_INPUT;
        }
    }

    // Проверка на проблемы
    if (len_mantissa == 0)
        return ERROR_INPUT;

    // Завершение строки-мантиссы
    number->mantissa[len_mantissa] = '\0';

    // Реальный порядок с учетом точки
    if (pos_dot == -1) 
        pos_dot = len_mantissa;
    if (number->sign_exponent == '-') 
        number->exponent = -number->exponent;
    number->exponent = number->exponent + pos_dot;

    // Нормализация порядка
    if (number->exponent >= 0) 
    {
        number->exponent = number->exponent;
        number->sign_exponent = '+';
    } 
    else 
    {
        number->exponent = -(number->exponent);
        number->sign_exponent = '-';
    }

    // Удаление ведущих нулей и коррекция экспоненты
    delete_lead_zeros(number);

    // Смена знака экспоненты
    if (number->exponent < 0)
    {
        number->exponent = -(number->exponent);
        if (number->sign_exponent == '+')
            number->sign_exponent = '-';
        else
            number->sign_exponent = '+';
    }

    // Удаление заключительных нулей
    delete_end_zeros(number);

    return 0;
}

void print_number(const number *number)
{
    printf("%c0.%sE%c%d\n", number->sign_mantissa, number->mantissa, number->sign_exponent, number->exponent);
}

int multiplication(const number *a, const number *b, number *result)
{
    // Инициализация результата
    result->sign_mantissa = (a->sign_mantissa == b->sign_mantissa) ? '+' : '-';
    result->sign_exponent = '+';
    result->exponent = 0;
    
    // Вычисление результирующего порядка
    int exp_a = (a->sign_exponent == '+') ? a->exponent : -a->exponent;
    int exp_b = (b->sign_exponent == '+') ? b->exponent : -b->exponent;
    result->exponent = exp_a + exp_b - 1;
    
    if (result->exponent >= 0)
    {
        result->exponent = result->exponent;
        result->sign_exponent = '+';
    }
    else
    {
        result->exponent = -result->exponent;
        result->sign_exponent = '-';
    }
    
    // Получение длин мантисс
    size_t len_a = strlen(a->mantissa);
    size_t len_b = strlen(b->mantissa);
    
    // Если одно из чисел ноль
    if ((len_a == 1 && a->mantissa[0] == '0') || (len_b == 1 && b->mantissa[0] == '0'))
    {
        strcpy(result->mantissa, "0");
        result->exponent = 0;
        result->sign_exponent = '+';
        result->sign_mantissa = '+';
        return OK;
    }
    
    // Создание временного массива для результата умножения
    int temp_result[MAX_LEN_MANTISSA * 2] = {0};
    int product, pos, k;

    // Умножение в столбик
    for (int i = len_a - 1; i >= 0; i--)
    {
        for (int j = len_b - 1; j >= 0; j--)
        {
            product = (a->mantissa[i] - '0') * (b->mantissa[j] - '0');
            pos = (len_a - 1 - i) + (len_b - 1 - j);
            temp_result[pos] += product;
            
            // Обработка переносов
            k = pos;
            while (temp_result[k] >= 10)
            {
                temp_result[k + 1] += temp_result[k] / 10;
                temp_result[k] %= 10;
                k++;
            }
        }
    }
    
    // Определение длины результата
    size_t result_len = MAX_LEN_MANTISSA * 2;
    while (result_len > 0 && temp_result[result_len - 1] == 0)
        result_len--;
    
    if (result_len == 0)
    {
        strcpy(result->mantissa, "0");
        return OK;
    }

    // Перенос
    if (result_len > len_a + len_b - 1)
        result->exponent += 1;
    
    // Преобразование в строку и нормализация
    char temp_str[MAX_LEN_MANTISSA * 2 + 1];
    for (size_t i = 0; i < result_len; i++)
        temp_str[i] = temp_result[result_len - 1 - i] + '0';
    temp_str[result_len] = '\0';
    
    // Округление если результат длиннее максимального
    if (result_len > MAX_LEN_RESULT)
    {
        // Проверка на необходимость округления
        if (temp_str[MAX_LEN_RESULT] >= '5')
        {
            // Округление вверх
            int carry = 1, digit;

            for (int i = MAX_LEN_RESULT - 1; i >= 0 && carry; i--)
            {
                digit = (temp_str[i] - '0') + carry;
                temp_str[i] = (digit % 10) + '0';
                carry = digit / 10;
            }
            
            if (carry)
            {
                // Сдвиг вправо и увеличение порядка
                for (int i = MAX_LEN_RESULT; i > 0; i--)
                    temp_str[i] = temp_str[i - 1];
                temp_str[0] = '1';
                result->exponent++;
                
                // Проверка переполнения порядка после округления
                if (result->exponent > 99999)
                    return ERROR_OVERFLOW;
            }
        }
        temp_str[MAX_LEN_RESULT] = '\0';
    }
    
    // Копирование результата
    strncpy(result->mantissa, temp_str, MAX_LEN_RESULT);
    result->mantissa[MAX_LEN_RESULT] = '\0';
    
    // Удаление ведущих нулей
    delete_lead_zeros(result);

    // Удаление заключительных нулей
    delete_end_zeros(result);

    // Проверка переполнения порядка
    if (result->exponent > 99999)
        return ERROR_OVERFLOW;
    if (result->exponent < -99999)
        return ERROR_UNDERFLOW;
    
    return OK;
}

void delete_lead_zeros(number *number)
{
    size_t leading_zeros = 0;
    while (number->mantissa[leading_zeros] == '0' && number->mantissa[leading_zeros] != '\0') 
        leading_zeros++;

    if (leading_zeros > 0) 
    {
        // Сдвиг мантиссы влево
        size_t new_len = strlen(number->mantissa) - leading_zeros;
        if (new_len == 0) 
        {
            strcpy(number->mantissa, "0");
            number->exponent = 0;
            number->sign_exponent = '+';
        } 
        else 
        {
            for (size_t i = 0; i <= new_len; i++)
                number->mantissa[i] = number->mantissa[i + leading_zeros];
            
            if (number->sign_exponent == '+')
                number->exponent -= leading_zeros;
            else
                number->exponent += leading_zeros;
        }
    }
}

void delete_end_zeros(number *number)
{
    size_t len = strlen(number->mantissa);
    if (len > 1)
    {
        for (size_t i = len - 1; number->mantissa[i] == '0'; i--)
            number->mantissa[i] = '\0';
    }
}