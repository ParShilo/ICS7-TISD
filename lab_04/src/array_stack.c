#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../inc/err.h"
#include "stack.h"
#include "main.h"
#include "print.h"
#include "array_stack.h"

// Функция взаимодействий со стеком в виде массива
void array_stack_operations(ArrayStack* stack)
{
    int operation, value;
    
    do {
        print_operation_menu(1);
        operation = get_integer_input("Введите номер", 0, 4);
        
        switch (operation) 
        {
            case 1: 
            {
                if (array_is_full(stack))
                    printf("Ошибка: стек переполнен!\n");
                else
                {
                    value = get_integer_input("Введите целое число", -100000, 100000);
                    array_push(stack, value);
                    printf("Элемент %d добавлен в стек.\n", value);
                }
                break;
            }
            case 2:
            {
                if (array_is_empty(stack))
                    printf("Ошибка: стек пуст!\n");
                else
                {
                    array_pop(stack, &value);
                    printf("Элемент %d удален из стека\n", value);
                }
                break;
            }
            case 3:
            {
                printf("Сортировка с помощью третьего стека.\n");
                sort_with_array_stack();
                break;
            }
            case 4:
            {
                array_display(stack);
                break;
            }
            case 0:
                printf("Возврат в предыдущее меню.\n");
                break;
            default:
                printf("Неверная операция!\n");
        }
    } while (operation != 0);
}

// Функция инициализации стека в виде массива
void array_init(ArrayStack* stack)
{
    stack->top = -1;
}

// Функция проверки стека на отсутствие элементов
int array_is_empty(ArrayStack* stack)
{
    return stack->top == -1;
}

// Функция проверки стека на переполнение
int array_is_full(ArrayStack* stack)
{
    return stack->top == MAX_SIZE - 1;
}

// Функция для добавление нового элемента в стек
int array_push(ArrayStack* stack, int value)
{
    if (array_is_full(stack))
        return ERROR_OVERFLOW;

    stack->data[++stack->top] = value;
    return ERROR_OK;
}

// Функция для удаления последнего элемента из стека
int array_pop(ArrayStack* stack, int* value)
{
    if (array_is_empty(stack))
        return ERROR_DELETING;

    *value = stack->data[stack->top--];
    return ERROR_OK;
}

// Функция вывода стека
void array_display(ArrayStack* stack)
{
    if (array_is_empty(stack))
    {
        printf("Стек пуст\n");
        return;
    }
    
    printf("Содержимое стека: ");
    for (int i = stack->top; i >= 0; i--)
        printf("%d ", stack->data[i]);
    printf("\n");
}

// Функция сортировки двух стеков с помощью третьего
void sort_with_array_stack(void)
{
    ArrayStack stack1, stack2, temp, result;
    int value;
    array_init(&stack1);
    array_init(&stack2);
    array_init(&temp);
    array_init(&result);
    
    printf("=== Сортировка с использованием массива ===\n");
    
    // Ввод первого стека
    int n1 = get_integer_input("Введите количество элементов для первого стека", 1, MAX_SIZE/2);
    printf("Введите элементы первого стека:\n");
    for (int i = 0; i < n1; i++)
    {
        value = get_integer_input("", -100000, 100000);
        array_push(&stack1, value);
    }
    
    // Ввод второго стека
    int n2 = get_integer_input("Введите количество элементов для второго стека", 1, MAX_SIZE/2);
    printf("Введите элементы второго стека:\n");
    for (int i = 0; i < n2; i++)
    {
        value = get_integer_input("", -100000, 100000);
        array_push(&stack2, value);
    }
    
    // Объединение и сортировка
    while (!array_is_empty(&stack1))
    {
        array_pop(&stack1, &value);
        array_push(&temp, value);
    }
    while (!array_is_empty(&stack2))
    {
        array_pop(&stack2, &value);
        array_push(&temp, value);
    }
    
    // Сортировка
    int temp_val;
    while (!array_is_empty(&temp))
    {
        array_pop(&temp, &value);
        while (!array_is_empty(&result) && result.data[result.top] < value)
        {
            array_pop(&result, &temp_val);
            array_push(&temp, temp_val);
        }
        array_push(&result, value);
    }
    
    printf("Отсортированный стек: ");
    array_display(&result);
}