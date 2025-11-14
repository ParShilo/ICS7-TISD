#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../inc/err.h"
#include "stack.h"
#include "main.h"
#include "print.h"
#include "list_stack.h"

Node* free_addresses = NULL;
int free_count = 0;

// Функция взаимодействий со стеком в виде списка
void list_stack_operations(ListStack* stack)
{
    int operation, value;
    
    do {
        print_operation_menu(2);
        operation = get_integer_input("Введите номер", 0, 6);
        
        switch (operation)
        {
            case 1:
            {
                if (list_is_full(stack))
                    printf("Ошибка: стек переполнен!\n");
                else
                {
                    value = get_integer_input("Введите целое число", -100000, 100000);
                    if (list_push(stack, value) == ERROR_OK)
                        printf("Элемент %d добавлен в стек\n", value);
                    else
                        printf("Ошибка: Не удалось выделить память!\n");
                }
                break;
            }
            case 2:
            {
                if (list_is_empty(stack))
                    printf("Ошибка: стек пуст!\n");
                else
                {
                    list_pop(stack, &value);
                    printf("Элемент %d удален из стека\n", value);
                }
                break;
            }
            case 3:
            {
                printf("Сортировка с помощью третьего стека.\n");
                sort_with_list_stack();
                break;
            }
            case 4:
            {
                list_display(stack);
                break;
            }
            case 5:
            {
                list_display_with_addresses(stack);
                break;
            }
            case 6:
            {
                display_free_addresses();
                break;
            }
            case 0:
            {
                printf("Возврат в предыдущее меню.\n");
                break;
            }
            default:
                printf("Неверная операция!\n");
        }
    } while (operation != 0);
}

// Функция инициализации стека в виде списка
void list_init(ListStack* stack)
{
    stack->top = NULL;
    stack->size = 0;
}

// Функция проверки стека на отсутствие элементов
int list_is_empty(ListStack* stack)
{
    return stack->top == NULL;
}

// Функция проверки стека на переполнение
int list_is_full(ListStack* stack)
{
    return stack->size == MAX_SIZE;
}

// Функция для добавление нового элемента в стек
int list_push(ListStack* stack, int value)
{
    if (list_is_full(stack))
        return ERROR_OVERFLOW;

    Node* new_node = malloc(sizeof(Node));
    if (!new_node)
        return ERROR_MEM;
    
    new_node->data = value;
    new_node->next = stack->top;
    stack->top = new_node;
    stack->size++;
    
    return ERROR_OK;
}

// Функция для удаления последнего элемента из стека
int list_pop(ListStack* stack, int* value)
{
    if (list_is_empty(stack))
        return ERROR_DELETING;
    
    Node* temp = stack->top;
    *value = temp->data;
    stack->top = temp->next;
    
    add_free_address(temp);
    free(temp);
    stack->size--;
    
    return ERROR_OK;
}

// Функция вывода стека
void list_display(ListStack* stack)
{
    if (list_is_empty(stack))
    {
        printf("Стек пуст\n");
        return;
    }
    
    printf("Содержимое стека : ");
    Node* current = stack->top;
    while (current != NULL)
    {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Функция вывода стека с адресами
void list_display_with_addresses(ListStack* stack)
{
    if (list_is_empty(stack))
    {
        printf("Стек пуст\n");
        return;
    }
    
    printf("Содержимое стека с адресами:\n");
    Node* current = stack->top;
    //printf("Размер: %ld\n", sizeof(Node));
    while (current != NULL)
    {
        printf("Адрес: %p, Данные: %d\n", (void*)current, current->data);
        current = current->next;
    }
}

// Функция освобождения стека в виде списка
void list_free(ListStack* stack)
{
    int value;
    while (!list_is_empty(stack))
        list_pop(stack, &value);
}

// Функция сортировки двух стеков с помощью третьего
void sort_with_list_stack(void)
{
    ListStack stack1, stack2, temp, result;
    int value;
    list_init(&stack1);
    list_init(&stack2);
    list_init(&temp);
    list_init(&result);
    
    printf("=== Сортировка с использованием списка ===\n");
    
    // Ввод первого стека
    int n1 = get_integer_input("Введите количество элементов для первого стека", 1, MAX_SIZE / 2);
    printf("Введите элементы первого стека:\n");
    for (int i = 0; i < n1; i++)
    {
        value = get_integer_input("", -100000, 100000);
        list_push(&stack1, value);
    }
    
    // Ввод второго стека
    int n2 = get_integer_input("Введите количество элементов для второго стека", 1, MAX_SIZE / 2);
    printf("Введите элементы второго стека:\n");
    for (int i = 0; i < n2; i++)
    {
        value = get_integer_input("", -100000, 100000);
        list_push(&stack2, value);
    }
    
    // Объединение и сортировка
    while (!list_is_empty(&stack1))
    {
        list_pop(&stack1, &value);
        list_push(&temp, value);
    }
    while (!list_is_empty(&stack2))
    {
        list_pop(&stack2, &value);
        list_push(&temp, value);
    }
    
    // Сортировка
    int temp_val;
    while (!list_is_empty(&temp))
    {
        list_pop(&temp, &value);
        while (!list_is_empty(&result) && result.top->data < value)
        {
            list_pop(&result, &temp_val);
            list_push(&temp, temp_val);
        }
        list_push(&result, value);
    }
    
    printf("Отсортированный стек: ");
    list_display(&result);
    
    // Очистка
    list_free(&stack1);
    list_free(&stack2);
    list_free(&temp);
    list_free(&result);
}

// Функции добавления свободных адресов
void add_free_address(void* addr)
{
    (void)addr;
    Node* new_node = malloc(sizeof(Node));
    if (new_node)
    {
        new_node->data = 0;
        new_node->next = free_addresses;
        free_addresses = new_node;
        free_count++;
    }
}

// Функция вывода свободных адресов
void display_free_addresses(void)
{
    if (free_addresses == NULL)
    {
        printf("История удалений пуста\n");
        return;
    }
    
    printf("История удалений (%d элементов):\n", free_count);
    Node* current = free_addresses;
    while (current != NULL)
    {
        printf("Освобожденный адрес: %p\n", (void*)current);
        current = current->next;
    }
}

// Функция очистки свободных адресов
void clear_free_addresses(void)
{
    Node* temp = NULL;
    while (free_addresses != NULL)
    {
        temp = free_addresses;
        free_addresses = free_addresses->next;
        free(temp);
    }
    free_count = 0;
}