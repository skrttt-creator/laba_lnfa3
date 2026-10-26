#include "Algorithms.h"

static int g_targetNumberForDivisors = 1;

Stack<long long>* GenerateRecurrent(
    const Stack<long long>& initialTerms,
    int totalCount,
    long long (*next)(const Stack<long long>&, size_t)
) {
    if (next == nullptr) {
        throw InvalidArgument("Указатель на порождающую функцию равен nullptr");
    }
    if (totalCount < 0) {
        throw InvalidArgument("Количество элементов не может быть отрицательным");
    }

    size_t initialLen = initialTerms.GetCount();
    size_t targetLen = static_cast<size_t>(totalCount);

    if (targetLen < initialLen) {
        throw InvalidArgument("Итоговая длина меньше количества начальных элементов");
    }

    Stack<long long>* result = new Stack<long long>(initialTerms.IsUsingList());

    try {
        
        for (size_t i = 0; i < initialLen; ++i) {
            result->Push(initialTerms.Get(i));
        }

        
        for (size_t n = initialLen; n < targetLen; ++n) {
            long long nextVal = next(*result, n);
            result->Push(nextVal);
        }
    } catch (...) {
        
        
        delete result;
        throw;
    }

    return result;
}

long long FactorialNext(const Stack<long long>& history, size_t n) {
    if (n >= 20) {
        throw InvalidArgument("Переполнение типа long long при вычислении факториала (n > 20)");
    }
    
    return history.Get(n - 1) * static_cast<long long>(n + 1);
}

long long FibonacciNext(const Stack<long long>& history, size_t n) {
    if (n >= 92) {
        throw InvalidArgument("Переполнение типа long long при вычислении чисел Фибоначчи (n > 92)");
    }
    return history.Get(n - 1) + history.Get(n - 2);
}


Stack<long long>* GenerateFactorials(int count) {
    if (count < 0) {
        throw InvalidArgument("Количество элементов факториала не может быть отрицательным");
    }
    if (count == 0) {
        return new Stack<long long>();
    }

    Stack<long long> initialTerms;
    initialTerms.Push(1); 

    return GenerateRecurrent(initialTerms, count, FactorialNext);
}


Stack<long long>* GenerateFibonacci(int count) {
    if (count < 0) {
        throw InvalidArgument("Количество чисел Фибоначчи не может быть отрицательным");
    }
    if (count == 0) {
        return new Stack<long long>();
    }

    Stack<long long> initialTerms;
    initialTerms.Push(1); 
    if (count == 1) {
        return new Stack<long long>(initialTerms);
    }

    initialTerms.Push(1); 
    return GenerateRecurrent(initialTerms, count, FibonacciNext);
}


Stack<long long>* GenerateLinearRecurrent(
    const Stack<long long>& initialTerms,
    const Stack<long long>& coefficients,
    int totalCount
) {
    if (totalCount < 0) {
        throw InvalidArgument("Количество элементов не может быть отрицательным");
    }

    size_t k = coefficients.GetCount();
    if (k == 0) {
        throw InvalidArgument("Список коэффициентов не может быть пустым (порядок k >= 1)");
    }
    if (initialTerms.GetCount() != k) {
        throw InvalidArgument("Количество начальных элементов должно совпадать с порядком рекурренты k");
    }
    if (coefficients.Get(k - 1) == 0) {
        throw InvalidArgument("Старший коэффициент a_k не может быть равен 0 по определению порядка k");
    }

    size_t targetLen = static_cast<size_t>(totalCount);
    if (targetLen < k) {
        throw InvalidArgument("Итоговая длина не может быть меньше порядка рекурренты k");
    }

    Stack<long long>* result = new Stack<long long>(initialTerms.IsUsingList());

    try {
        
        for (size_t i = 0; i < k; ++i) {
            result->Push(initialTerms.Get(i));
        }

        
        for (size_t n = k; n < targetLen; ++n) {
            long long sum = 0;
            for (size_t j = 0; j < k; ++j) {
                sum += coefficients.Get(j) * result->Get(n - 1 - j);
            }
            result->Push(sum);
        }
    } catch (...) {
        delete result;
        throw;
    }

    return result;
}







Stack<int>* GenerateRange(int l, int h) {
    if (l > h) {
        throw InvalidArgument("Левая граница диапазона не может быть больше правой (l > h)");
    }

    Stack<int>* result = new Stack<int>();
    try {
        for (int i = l; i <= h; ++i) {
            result->Push(i);
        }
    } catch (...) {
        delete result;
        throw;
    }

    return result;
}

bool IsPrime(int x) {
    if (x <= 1) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;

    for (int i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

bool IsDivisor(int x) {
    if (x <= 0) return false;
    return (g_targetNumberForDivisors % x) == 0;
}


Stack<int>* GetDivisors(int n) {
    if (n <= 0) {
        throw InvalidArgument("Число для поиска множителей должно быть натуральным (n > 0)");
    }

    g_targetNumberForDivisors = n;

    
    Stack<int>* candidates = GenerateRange(1, n);

    
    Stack<int>* divisors = nullptr;
    try {
        divisors = candidates->Where(IsDivisor);
    } catch (...) {
        delete candidates;
        throw;
    }

    delete candidates;
    return divisors;
}


Stack<int>* GeneratePrimes(int l, int h) {
    if (l > h) {
        throw InvalidArgument("Левая граница диапазона не может быть больше правой (l > h)");
    }
    if (h < 2) {
        return new Stack<int>(); 
    }

    int start = (l < 2) ? 2 : l;

    
    Stack<int>* candidates = GenerateRange(start, h);

    
    Stack<int>* primes = nullptr;
    try {
        primes = candidates->Where(IsPrime);
    } catch (...) {
        delete candidates;
        throw;
    }

    delete candidates;
    return primes;
}