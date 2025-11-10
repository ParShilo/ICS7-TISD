#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../inc/err.h"
#include "stack.h"
#include "main.h"
#include "print.h"
#include "array_stack.h"

void array_stack_operations(ArrayStack* stack)
{
    int operation;
    
    do {
        print_operation_menu(1);
        operation = get_integer_input("Введите номер", 0, 4);
        
        switch (operation) 
        {
            case 1: 
            {
                int value = get_integer_input("Введите целое число", -100000, 100000);
                if (array_push(stack, value))
                    printf("Элемент %d добавлен в стек.\n", value);
                else
                    printf("Ошибка: стек переполнен!\n");
                break;
            }
            case 2:
            {
                int value;
                if (array_pop(stack, &value))
                    printf("Элемент %d удален из стека.\n", value);
                else
                    printf("Ошибка: стек пуст!\n");
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

// Реализация функций для массива
void array_init(ArrayStack* stack)
{
    stack->top = -1;
}

int array_is_empty(ArrayStack* stack)
{
    return stack->top == -1;
}

int array_is_full(ArrayStack* stack)
{
    return stack->top == MAX_SIZE - 1;
}

int array_push(ArrayStack* stack, int value)
{
    if (array_is_full(stack))
        return 0;
    stack->data[++stack->top] = value;
    return 1;
}

int array_pop(ArrayStack* stack, int* value)
{
    if (array_is_empty(stack))
        return 0;

    *value = stack->data[stack->top--];
    return 1;
}

void array_display(ArrayStack* stack)
{
    if (array_is_empty(stack))
    {
        printf("Стек пуст\n");
        return;
    }
    
    printf("Содержимое стека (сверху вниз): ");
    for (int i = stack->top; i >= 0; i--)
        printf("%d ", stack->data[i]);
    printf("\n");
}

void sort_with_array_stack(void)
{
    ArrayStack stack1, stack2, temp, result;
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
        int value = get_integer_input("", -100000, 100000);
        array_push(&stack1, value);
    }
    
    // Ввод второго стека
    int n2 = get_integer_input("Введите количество элементов для второго стека", 1, MAX_SIZE/2);
    printf("Введите элементы второго стека:\n");
    for (int i = 0; i < n2; i++)
    {
        int value = get_integer_input("", -100000, 100000);
        array_push(&stack2, value);
    }
    
    // Объединение и сортировка
    int value;
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
    while (!array_is_empty(&temp))
    {
        array_pop(&temp, &value);
        while (!array_is_empty(&result) && result.data[result.top] < value)
        {
            int temp_val;
            array_pop(&result, &temp_val);
            array_push(&temp, temp_val);
        }
        array_push(&result, value);
    }
    
    printf("Отсортированный стек: ");
    array_display(&result);
}