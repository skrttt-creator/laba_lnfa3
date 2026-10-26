#pragma once

#include "Stack.h"
#include "Exceptions.h"
#include <cstddef>

Stack<long long>* GenerateRecurrent(
    const Stack<long long>& initialTerms,
    int totalCount,
    long long (*next)(const Stack<long long>&, size_t)
);


long long FactorialNext(const Stack<long long>& history, size_t n);
long long FibonacciNext(const Stack<long long>& history, size_t n);


Stack<long long>* GenerateFactorials(int count);
Stack<long long>* GenerateFibonacci(int count);


Stack<long long>* GenerateLinearRecurrent(
    const Stack<long long>& initialTerms,
    const Stack<long long>& coefficients,
    int totalCount
);


Stack<int>* GenerateRange(int l, int h);

bool IsPrime(int x);
bool IsDivisor(int x);

Stack<int>* GetDivisors(int n);
Stack<int>* GeneratePrimes(int l, int h);