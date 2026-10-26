#pragma once

#include "Sequence.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "Exceptions.h"
#include "ICollection.h"
#include "IEnumerable.h"
#include <cstddef>

template <class T>
class Stack : public ICollection<T>, public IEnumerable<T> {
private:
    Sequence<T>* seq;
    bool useList;

    Sequence<T>* CreateEmptySequence() const;

public:
    explicit Stack(bool useListBackend = false);
    Stack(const T* items, size_t count, bool useListBackend = false);
    explicit Stack(const Sequence<T>* otherSeq, bool useListBackend = false);
    Stack(const Stack<T>& other);
    Stack<T>& operator=(const Stack<T>& other);
    ~Stack() override;

    void Push(const T& item);
    T Pop();
    T Peek() const;
    bool IsEmpty() const;
    
    size_t GetCount() const override;
    T Get(size_t index) const override;
    
    bool IsUsingList() const;

    Stack<T>* Map(T (*func)(T)) const;
    Stack<T>* Where(bool (*func)(T)) const;
    T Reduce(T (*func)(T, T), T initial) const;

    Stack<T>* Concat(const Stack<T>& other) const;
    Stack<T>* GetSubsequence(size_t startIndex, size_t endIndex) const;
    
    int FindSubsequence(const Sequence<T>* sub) const;
    int FindSubsequence(const Stack<T>& sub) const;
    bool ContainsSubsequence(const Sequence<T>* sub) const;
    bool ContainsSubsequence(const Stack<T>& sub) const;

    Stack<T> operator+(const Stack<T>& other) const;
    Stack<T>& operator<<(const T& item);
    bool operator==(const Stack<T>& other) const;
    bool operator!=(const Stack<T>& other) const;
    T operator[](size_t index) const;

    IEnumerator<T>* GetEnumerator() const override;
};

#include "Stack.tpp"