#include "Tests.h"
#include "Stack.h"
#include "Algorithms.h"
#include "Exceptions.h"

#include <stdio.h>
#include <cassert>
#include <chrono>

// Вспомогательные функции для тестирования Map, Where, Reduce
static int TestMapTriple(int x) { return x * 3; }
static bool TestWhereOdd(int x) { return x % 2 != 0; }
static int TestReduceSum(int acc, int x) { return acc + x; }


// ТЕСТЫ ЦЕЛЕВОГО АТД СТЕК, ICOLLECTION И ОПЕРАТОРОВ


static void TestStackBasicOperations(bool useList) {
    Stack<int> s(useList);
    assert(s.IsEmpty());
    assert(s.GetCount() == 0);

    s.Push(10);
    s.Push(20);
    s.Push(30);

    assert(!s.IsEmpty());
    assert(s.GetCount() == 3);
    assert(s.Peek() == 30);

    assert(s.Pop() == 30);
    assert(s.GetCount() == 2);
    assert(s.Peek() == 20);

    assert(s.Pop() == 20);
    assert(s.Pop() == 10);
    assert(s.IsEmpty());
}

static void TestICollectionAndCopy() {
    int raw[] = {5, 10, 15, 20};
    Stack<int> original(raw, 4, false);

    // Проверка копирующего конструктора
    Stack<int> copyConstructed(original);
    assert(copyConstructed.GetCount() == 4);
    for (size_t i = 0; i < 4; ++i) {
        assert(copyConstructed.Get(i) == raw[i]);
    }

    // Проверка полиморфного обращения через интерфейс ICollection<T>
    const ICollection<int>* collectionPtr = &copyConstructed;
    assert(collectionPtr->GetCount() == 4);
    assert(collectionPtr->Get(0) == 5);
    assert(collectionPtr->Get(3) == 20);
}

static void TestStackOperators() {
    Stack<int> s1;
    s1 << 1 << 2 << 3;
    assert(s1.GetCount() == 3);
    assert(s1[0] == 1 && s1[1] == 2 && s1[2] == 3);

    Stack<int> s2;
    s2 << 4 << 5;

    // Оператор сложения (конкатенация)
    Stack<int> combined = s1 + s2;
    assert(combined.GetCount() == 5);
    assert(combined[0] == 1 && combined[4] == 5);

    // Операторы сравнения == и !=
    Stack<int> s1Copy(s1);
    assert(s1 == s1Copy);
    assert(s1 != s2);
}

static void TestStackSubsequenceAndSearch() {
    int raw[] = {10, 20, 30, 40, 50, 60};
    Stack<int> s(raw, 6);

    // Извлечение подпоследовательности по индексам
    Stack<int>* sub = s.GetSubsequence(1, 3);
    assert(sub->GetCount() == 3);
    assert(sub->Get(0) == 20 && sub->Get(1) == 30 && sub->Get(2) == 40);

    // Поиск на вхождение подпоследовательности
    assert(s.ContainsSubsequence(*sub));
    assert(s.FindSubsequence(*sub) == 1);

    Stack<int> notFoundSub;
    notFoundSub << 20 << 99;
    assert(!s.ContainsSubsequence(notFoundSub));
    assert(s.FindSubsequence(notFoundSub) == -1);

    delete sub;
}

static void TestStackHigherOrderFunctions() {
    int raw[] = {1, 2, 3, 4, 5};
    Stack<int> s(raw, 5);

    // Map
    Stack<int>* mapped = s.Map(TestMapTriple);
    assert(mapped->GetCount() == 5);
    assert(mapped->Get(0) == 3 && mapped->Get(4) == 15);

    // Where
    Stack<int>* filtered = s.Where(TestWhereOdd);
    assert(filtered->GetCount() == 3);
    assert(filtered->Get(0) == 1 && filtered->Get(1) == 3 && filtered->Get(2) == 5);

    // Reduce
    int sum = s.Reduce(TestReduceSum, 0);
    assert(sum == 15);

    delete mapped;
    delete filtered;
}

static void TestStackExceptions() {
    Stack<int> emptyStack;
    bool caughtPop = false;
    try {
        emptyStack.Pop();
    } catch (const IndexOutOfRange&) {
        caughtPop = true;
    }
    assert(caughtPop);

    bool caughtPeek = false;
    try {
        emptyStack.Peek();
    } catch (const IndexOutOfRange&) {
        caughtPeek = true;
    }
    assert(caughtPeek);

    emptyStack.Push(42);
    bool caughtOutOfBounds = false;
    try {
        emptyStack.Get(5);
    } catch (const IndexOutOfRange&) {
        caughtOutOfBounds = true;
    }
    assert(caughtOutOfBounds);
}

void RunStackTests() {
    printf("\nЗАПУСК МОДУЛЬНЫХ ТЕСТОВ СТЕКА \n");

    TestStackBasicOperations(false);
    printf("[OK] Базовые операции Push/Pop/Peek (ArraySequence)\n");

    TestStackBasicOperations(true);
    printf("[OK] Базовые операции Push/Pop/Peek (ListSequence)\n");

    TestICollectionAndCopy();
    printf("[OK] Интерфейс ICollection<T> и копирующий конструктор \n");

    TestStackOperators();
    printf("[OK] Перегрузка операторов <<, +, ==, !=, [] \n");

    TestStackSubsequenceAndSearch();
    printf("[OK] Извлечение и поиск подпоследовательности\n");

    TestStackHigherOrderFunctions();
    printf("[OK] Функции высшего порядка Map, Where, Reduce\n");

    TestStackExceptions();
    printf("[OK] Обработка исключительных ситуаций (IndexOutOfRange)\n");

    printf(" ВСЕ ТЕСТЫ СТЕКА УСПЕШНО ПРОЙДЕНЫ\n");
}


// ТЕСТЫ АЛГОРИТМОВ

static void TestRecurrentFactorials() {
    // тест факториалов 1! = 1, 2! = 2, 3! = 6, 4! = 24, 5! = 120
    Stack<long long>* fact = GenerateFactorials(5);
    assert(fact->GetCount() == 5);
    assert(fact->Get(0) == 1);
    assert(fact->Get(1) == 2);
    assert(fact->Get(2) == 6);
    assert(fact->Get(3) == 24);
    assert(fact->Get(4) == 120);
    delete fact;

    // Проверка защиты от переполнения long long (n > 20)
    bool overflowCaught = false;
    try {
        Stack<long long>* huge = GenerateFactorials(25);
        delete huge;
    } catch (const InvalidArgument&) {
        overflowCaught = true;
    }
    assert(overflowCaught);
}

static void TestRecurrentFibonacci() {
    // фибоначчи 1, 1, 2, 3, 5, 8, 13
    Stack<long long>* fib = GenerateFibonacci(7);
    assert(fib->GetCount() == 7);
    assert(fib->Get(0) == 1);
    assert(fib->Get(1) == 1);
    assert(fib->Get(2) == 2);
    assert(fib->Get(3) == 3);
    assert(fib->Get(4) == 5);
    assert(fib->Get(5) == 8);
    assert(fib->Get(6) == 13);
    delete fib;
}

static void TestLinearRecurrentGeneral() {
    // Проверим рекурренту 3-го порядка: x_n = 1*x_{n-1} + 2*x_{n-2} + 3*x_{n-3}
    // Начальные: x_0 = 1, x_1 = 2, x_2 = 3
    // x_3 = 1*3 + 2*2 + 3*1 = 3 + 4 + 3 = 10
    // x_4 = 1*10 + 2*3 + 3*2 = 10 + 6 + 6 = 22
    Stack<long long> init;
    init << 1 << 2 << 3;

    Stack<long long> coeffs;
    coeffs << 1 << 2 << 3;

    Stack<long long>* seq = GenerateLinearRecurrent(init, coeffs, 5);
    assert(seq->GetCount() == 5);
    assert(seq->Get(0) == 1);
    assert(seq->Get(1) == 2);
    assert(seq->Get(2) == 3);
    assert(seq->Get(3) == 10);
    assert(seq->Get(4) == 22);
    delete seq;

    // Проверка исключения при старшем коэффициенте a_k == 0
    Stack<long long> badCoeffs;
    badCoeffs << 1 << 2 << 0;
    bool zeroCoeffCaught = false;
    try {
        Stack<long long>* bad = GenerateLinearRecurrent(init, badCoeffs, 5);
        delete bad;
    } catch (const InvalidArgument&) {
        zeroCoeffCaught = true;
    }
    assert(zeroCoeffCaught);
}

static void TestRangeAndNumberTheory() {
    // Range [3, 7] -> 3, 4, 5, 6, 7
    Stack<int>* range = GenerateRange(3, 7);
    assert(range->GetCount() == 5);
    assert(range->Get(0) == 3 && range->Get(4) == 7);
    delete range;

    // Делители числа 28 -> 1, 2, 4, 7, 14, 28
    Stack<int>* divs = GetDivisors(28);
    int expectedDivs[] = {1, 2, 4, 7, 14, 28};
    assert(divs->GetCount() == 6);
    for (size_t i = 0; i < 6; ++i) {
        assert(divs->Get(i) == expectedDivs[i]);
    }
    delete divs;

    // Простые числа на [1, 20] -> 2, 3, 5, 7, 11, 13, 17, 19
    Stack<int>* primes = GeneratePrimes(1, 20);
    int expectedPrimes[] = {2, 3, 5, 7, 11, 13, 17, 19};
    assert(primes->GetCount() == 8);
    for (size_t i = 0; i < 8; ++i) {
        assert(primes->Get(i) == expectedPrimes[i]);
    }
    delete primes;
}

void RunAlgorithmTests() {
    printf("\n=== ЗАПУСК ТЕСТОВ АЛГОРИТМОВ \n");

    TestRecurrentFactorials();
    printf("[OK] А-2.1: Рекуррента 1-го порядка (Факториал + контроль переполнения)\n");

    TestRecurrentFibonacci();
    printf("[OK] А-2.2: Рекуррента 2-го порядка (Числа Фибоначчи)\n");

    TestLinearRecurrentGeneral();
    printf("[OK] А-2.3: Линейная рекуррента порядка k (список коэффициентов + проверка a_k != 0)\n");

    TestRangeAndNumberTheory();
    printf("[OK] А-3, А-3.1, А-3.2: Range, разложение на множители и простые числа через Where\n");

    printf("=== ВСЕ ТЕСТЫ АЛГОРИТМОВ УСПЕШНО ПРОЙДЕНЫ! ===\n");
}



// 3. ОЦЕНКА ПРОИЗВОДИТЕЛЬНОСТИ (ArraySequence vs ListSequence)


void CompareStackPerformance() {
    const int N = 5000;
    printf("\n ТЕСТ ПРОИЗВОДИТЕЛЬНОСТИ СТЕКА (%d элементов) \n", N);

    // 1. Стек на базе динамического массива (ArraySequence)
    Stack<int> arrStack(false);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        arrStack.Push(i);
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    long long sumArr = 0;
    for (size_t i = 0; i < arrStack.GetCount(); ++i) {
        sumArr += arrStack.Get(i);
    }
    auto t3 = std::chrono::high_resolution_clock::now();

    while (!arrStack.IsEmpty()) {
        arrStack.Pop();
    }
    auto t4 = std::chrono::high_resolution_clock::now();

    // 2. Стек на базе связного списка (ListSequence)
    Stack<int> listStack(true);
    auto t5 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        listStack.Push(i);
    }
    auto t6 = std::chrono::high_resolution_clock::now();

    long long sumList = 0;
    for (size_t i = 0; i < listStack.GetCount(); ++i) {
        sumList += listStack.Get(i);
    }
    auto t7 = std::chrono::high_resolution_clock::now();

    while (!listStack.IsEmpty()) {
        listStack.Pop();
    }
    auto t8 = std::chrono::high_resolution_clock::now();

    assert(sumArr == sumList);

    double arrPush = std::chrono::duration<double, std::milli>(t2 - t1).count();
    double arrGet  = std::chrono::duration<double, std::milli>(t3 - t2).count();
    double arrPop  = std::chrono::duration<double, std::milli>(t4 - t3).count();

    double listPush = std::chrono::duration<double, std::milli>(t6 - t5).count();
    double listGet  = std::chrono::duration<double, std::milli>(t7 - t6).count();
    double listPop  = std::chrono::duration<double, std::milli>(t8 - t7).count();

    printf("Подложка ArraySequence:\n");
    printf("  Push %d эл.: %8.3f мс | Get(i): %8.3f мс | Pop %d эл.: %8.3f мс\n", N, arrPush, arrGet, N, arrPop);
    printf("Подложка ListSequence:\n");
    printf("  Push %d эл.: %8.3f мс | Get(i): %8.3f мс | Pop %d эл.: %8.3f мс\n", N, listPush, listGet, N, listPop);
}