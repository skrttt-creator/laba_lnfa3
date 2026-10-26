#include <stdio.h>
#include <iostream>
#include <cstddef>

#ifdef _WIN32
#include <windows.h>
#undef GetMessage
#endif

#include "Stack.h"
#include "Algorithms.h"
#include "Tests.h"
#include "Exceptions.h"


int MapMultiplyBy2(int x) { return x * 2; }
int MapSquare(int x) { return x * x; }

bool WhereIsEven(int x) { return x % 2 == 0; }
bool WhereIsPositive(int x) { return x > 0; }

int ReduceSum(int acc, int x) { return acc + x; }
int ReduceProduct(int acc, int x) { return acc * x; }

template <class T>
void PrintStack(const Stack<T>& stack) {
    printf("Текущий стек (count=%zu): [ ", stack.GetCount());

    IEnumerator<T>* it = stack.GetEnumerator();
    bool first = true;
    while (it->MoveNext()) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << it->GetCurrent();
        first = false;
    }
    delete it;

    printf(" ] <- вершина\n");
}

int ReadInt(const char* prompt) {
    printf("%s", prompt);
    int value = 0;
    while (scanf("%d", &value) != 1) {
        while (getchar() != '\n');
        printf("Ошибка! Введите целое число: ");
    }
    return value;
}

Stack<int> InputOtherStack(bool useList) {
    int count = ReadInt("Сколько элементов будет во втором стеке? ");
    if (count < 0) {
        throw InvalidArgument("Количество не может быть отрицательным");
    }

    Stack<int> other(useList);
    for (int i = 0; i < count; ++i) {
        printf("[%d] ", i);
        int val = ReadInt("Значение: ");
        other.Push(val);
    }
    return other;
}

void StackOperationsMenu(Stack<int>*& current) {
    int op = -1;
    while (op != 0) {
        printf("\n ОПЕРАЦИИ НАД СТЕКОМ \n");
        PrintStack(*current);
        printf("1.  Push / оператор << (добавить на вершину)\n");
        printf("2.  Pop (удалить и вернуть вершину)\n");
        printf("3.  Peek (посмотреть вершину)\n");
        printf("4.  Get(index) / [index]\n");
        printf("5.  GetSubsequence (извлечь подпоследовательность)\n");
        printf("6.  Concat / оператор + (сцепить со вторым стеком)\n");
        printf("7.  Поиск подпоследовательности (Find / Contains)\n");
        printf("8.  Сравнение стеков (операторы == и !=)\n");
        printf("9.  Map (x * 2 или x * x)\n");
        printf("10. Where (фильтр: четные или > 0)\n");
        printf("11. Reduce (сумма или произведение)\n");
        printf("0.  Вернуться в главное меню\n");

        op = ReadInt("Выберите операцию: ");

        try {
            switch (op) {
                case 1: {
                    int val = ReadInt("Значение: ");
                    printf("1. Push(val)\n2. Оператор <<\n");
                    int mode = ReadInt("Ваш выбор: ");
                    if (mode == 2) {
                        *current << val;
                    } else {
                        current->Push(val);
                    }
                    break;
                }
                case 2: {
                    int val = current->Pop();
                    printf("Извлечен элемент: %d\n", val);
                    break;
                }
                case 3: {
                    printf("Вершина стека (Peek): %d\n", current->Peek());
                    break;
                }
                case 4: {
                    int idx = ReadInt("Индекс: ");
                    if (idx < 0) throw IndexOutOfRange("Индекс не может быть отрицательным");
                    size_t uIdx = static_cast<size_t>(idx);
                    printf("Get(%zu) = %d, (*current)[%zu] = %d\n", uIdx, current->Get(uIdx), uIdx, (*current)[uIdx]);
                    break;
                }
                case 5: {
                    int start = ReadInt("Начальный индекс: ");
                    int end = ReadInt("Конечный индекс: ");
                    if (start < 0 || end < 0) throw IndexOutOfRange("Индекс не может быть отрицательным");
                    Stack<int>* sub = current->GetSubsequence(static_cast<size_t>(start), static_cast<size_t>(end));
                    printf("Результат:\n");
                    PrintStack(*sub);
                    delete sub;
                    break;
                }
                case 6: {
                    Stack<int> other = InputOtherStack(current->IsUsingList());
                    printf("1. Метод Concat\n2. Оператор +\n");
                    int mode = ReadInt("Ваш выбор: ");
                    if (mode == 2) {
                        Stack<int> res = *current + other;
                        delete current;
                        current = new Stack<int>(res);
                    } else {
                        Stack<int>* res = current->Concat(other);
                        delete current;
                        current = res;
                    }
                    printf("Сцепление выполнено!\n");
                    break;
                }
                case 7: {
                    Stack<int> sub = InputOtherStack(current->IsUsingList());
                    bool found = current->ContainsSubsequence(sub);
                    int pos = current->FindSubsequence(sub);
                    if (found) {
                        printf("Подпоследовательность входит в стек! Индекс начала: %d\n", pos);
                    } else {
                        printf("Подпоследовательность не найдена (результат: %d)\n", pos);
                    }
                    break;
                }
                case 8: {
                    Stack<int> other = InputOtherStack(current->IsUsingList());
                    printf("Равны (==): %s\n", (*current == other) ? "Да" : "Нет");
                    printf("Не равны (!=): %s\n", (*current != other) ? "Да" : "Нет");
                    break;
                }
                case 9: {
                    printf("Выберите функцию Map:\n1. Умножить на 2 (x * 2)\n2. Возвести в квадрат (x * x)\n");
                    int mode = ReadInt("Ваш выбор: ");
                    Stack<int>* mapped = (mode == 2) ? current->Map(MapSquare) : current->Map(MapMultiplyBy2);
                    delete current;
                    current = mapped;
                    printf("Map успешно применен!\n");
                    break;
                }
                case 10: {
                    printf("Выберите условие фильтрации Where:\n1. Оставить только четные\n2. Оставить только положительные (> 0)\n");
                    int mode = ReadInt("Ваш выбор: ");
                    Stack<int>* filtered = (mode == 2) ? current->Where(WhereIsPositive) : current->Where(WhereIsEven);
                    delete current;
                    current = filtered;
                    printf("Фильтрация Where выполнена!\n");
                    break;
                }
                case 11: {
                    if (current->IsEmpty()) throw IndexOutOfRange("Стек пуст");
                    printf("Выберите операцию Reduce:\n1. Сумма всех элементов\n2. Произведение всех элементов\n");
                    int mode = ReadInt("Ваш выбор: ");
                    int res = (mode == 2) ? current->Reduce(ReduceProduct, 1) : current->Reduce(ReduceSum, 0);
                    printf("Результат Reduce: %d\n", res);
                    break;
                }
                case 0:
                    break;
                default:
                    printf("Неверная операция.\n");
            }
        } catch (const Exception& e) {
            printf("\nОшибка: %s\n", e.GetMessage());
        }
    }
}

void AlgorithmsMenu() {
    int op = -1;
    while (op != 0) {
        printf("\n АЛГОРИТМЫ  \n");
        printf("1. А-2.1: Рекуррента 1-го порядка (Факториал)\n");
        printf("2. А-2.2: Рекуррента 2-го порядка (Числа Фибоначчи)\n");
        printf("3. А-2.3: Линейная рекуррента произвольного порядка k\n");
        printf("4. А-3:   Генерация ряда натуральных чисел Range(l, h)\n");
        printf("5. А-3.1: Разложение числа на множители (делители через Where)\n");
        printf("6. А-3.2: Определение простых чисел на отрезке [l, h] (через Where)\n");
        printf("0. Вернуться в главное меню\n");

        op = ReadInt("Выберите алгоритм: ");

        try {
            switch (op) {
                case 1: {
                    int n = ReadInt("Сколько факториалов сгенерировать (до 20)? ");
                    Stack<long long>* res = GenerateFactorials(n);
                    PrintStack(*res);
                    delete res;
                    break;
                }
                case 2: {
                    int n = ReadInt("Сколько чисел Фибоначчи сгенерировать (до 92)? ");
                    Stack<long long>* res = GenerateFibonacci(n);
                    PrintStack(*res);
                    delete res;
                    break;
                }
                case 3: {
                    int k = ReadInt("Введите порядок рекурренты k: ");
                    if (k <= 0) throw InvalidArgument("Порядок k должен быть больше 0");

                    Stack<long long> initTerms;
                    printf("Введите %d начальных элементов:\n", k);
                    for (int i = 0; i < k; ++i) {
                        printf("x[%d] = ", i);
                        initTerms.Push(static_cast<long long>(ReadInt("")));
                    }

                    Stack<long long> coeffs;
                    printf("Введите %d коэффициентов (a_1 ... a_k, где a_k != 0):\n", k);
                    for (int i = 0; i < k; ++i) {
                        printf("a_%d = ", i + 1);
                        coeffs.Push(static_cast<long long>(ReadInt("")));
                    }

                    int total = ReadInt("Введите общее количество элементов для генерации: ");
                    Stack<long long>* res = GenerateLinearRecurrent(initTerms, coeffs, total);
                    PrintStack(*res);
                    delete res;
                    break;
                }
                case 4: {
                    int l = ReadInt("Левая граница l: ");
                    int h = ReadInt("Правая граница h: ");
                    Stack<int>* res = GenerateRange(l, h);
                    PrintStack(*res);
                    delete res;
                    break;
                }
                case 5: {
                    int n = ReadInt("Введите натуральное число n: ");
                    Stack<int>* res = GetDivisors(n);
                    PrintStack(*res);
                    delete res;
                    break;
                }
                case 6: {
                    int l = ReadInt("Левая граница l: ");
                    int h = ReadInt("Правая граница h: ");
                    Stack<int>* res = GeneratePrimes(l, h);
                    PrintStack(*res);
                    delete res;
                    break;
                }
                case 0:
                    break;
                default:
                    printf("Неверный пункт.\n");
            }
        } catch (const Exception& e) {
            printf("\nОшибка: %s\n", e.GetMessage());
        }
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    Stack<int>* current = nullptr;
    int choice = -1;

    while (choice != 0) {
        printf("              Меню для работы                  \n");
        printf("1. Работать со Стеком (на базе динамического массива)\n");
        printf("2. Работать со Стеком (на базе связного списка)\n");
        printf("3. Алгоритмы раздела А (Рекурренты, Range, Делители, Простые)\n");
        printf("------------------------------------------------------\n");
        printf("4. Запустить тесты Стека\n");
        printf("5. Запустить тесты Алгоритмов (раздел А)\n");
        printf("6. Тест производительности Стека (Массив vs Список)\n");
        printf("0. Выход\n");

        choice = ReadInt("Ваш выбор: ");

        try {
            switch (choice) {
                case 1:
                    delete current;
                    current = new Stack<int>(false);
                    StackOperationsMenu(current);
                    break;
                case 2:
                    delete current;
                    current = new Stack<int>(true);
                    StackOperationsMenu(current);
                    break;
                case 3:
                    AlgorithmsMenu();
                    break;
                case 4:
                    RunStackTests();
                    break;
                case 5:
                    RunAlgorithmTests();
                    break;
                case 6:
                    CompareStackPerformance();
                    break;
                case 0:
                    printf("Выход из программы...\n");
                    break;
                default:
                    printf("Неверный пункт.\n");
            }
        } catch (const Exception& e) {
            printf("\nКритическая ошибка: %s\n", e.GetMessage());
        }
    }

    delete current;
    return 0;
}