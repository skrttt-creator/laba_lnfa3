#pragma once
#include "Stack.h"

template <class T>
Sequence<T>* Stack<T>::CreateEmptySequence() const {
    if (useList) {
        return new MutableListSequence<T>();
    }
    return new MutableArraySequence<T>();
}

template <class T>
Stack<T>::Stack(bool useListBackend) 
    : seq(nullptr), useList(useListBackend) {
    seq = CreateEmptySequence();
}

template <class T>
Stack<T>::Stack(const T* items, size_t count, bool useListBackend) 
    : seq(nullptr), useList(useListBackend) {
    if (useList) {
        seq = new MutableListSequence<T>(items, count);
    } else {
        seq = new MutableArraySequence<T>(items, count);
    }
}

template <class T>
Stack<T>::Stack(const Sequence<T>* otherSeq, bool useListBackend) 
    : seq(nullptr), useList(useListBackend) {
    if (otherSeq == nullptr) {
        throw InvalidArgument("Null sequence pointer");
    }
    seq = CreateEmptySequence();
    IEnumerator<T>* it = otherSeq->GetEnumerator();
    while (it->MoveNext()) {
        seq->Append(it->GetCurrent());
    }
    delete it;
}

template <class T>
Stack<T>::Stack(const Stack<T>& other) 
    : seq(nullptr), useList(other.useList) {
    seq = CreateEmptySequence();
    IEnumerator<T>* it = other.seq->GetEnumerator();
    while (it->MoveNext()) {
        seq->Append(it->GetCurrent());
    }
    delete it;
}

template <class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other) {
    if (this != &other) {
        delete seq;
        useList = other.useList;
        seq = CreateEmptySequence();
        IEnumerator<T>* it = other.seq->GetEnumerator();
        while (it->MoveNext()) {
            seq->Append(it->GetCurrent());
        }
        delete it;
    }
    return *this;
}

template <class T>
Stack<T>::~Stack() {
    delete seq;
}

template <class T>
void Stack<T>::Push(const T& item) {
    seq->Append(item);
}

template <class T>
T Stack<T>::Pop() {
    if (IsEmpty()) {
        throw IndexOutOfRange("Stack is empty");
    }

    size_t len = seq->GetCount();
    T topElement = seq->GetLast();

    if (len == 1) {
        delete seq;
        seq = CreateEmptySequence();
    } else {
        Sequence<T>* newSeq = seq->GetSubsequence(0, len - 2);
        delete seq;
        seq = newSeq;
    }

    return topElement;
}

template <class T>
T Stack<T>::Peek() const {
    if (IsEmpty()) {
        throw IndexOutOfRange("Stack is empty");
    }
    return seq->GetLast();
}

template <class T>
bool Stack<T>::IsEmpty() const {
    return seq->GetCount() == 0;
}

template <class T>
size_t Stack<T>::GetCount() const {
    return seq->GetCount();
}

template <class T>
T Stack<T>::Get(size_t index) const {
    return seq->Get(index);
}

template <class T>
bool Stack<T>::IsUsingList() const {
    return useList;
}

template <class T>
Stack<T>* Stack<T>::Map(T (*func)(T)) const {
    Sequence<T>* mappedSeq = seq->Map(func);
    Stack<T>* result = new Stack<T>(mappedSeq, useList);
    delete mappedSeq;
    return result;
}

template <class T>
Stack<T>* Stack<T>::Where(bool (*func)(T)) const {
    Sequence<T>* filteredSeq = seq->Where(func);
    Stack<T>* result = new Stack<T>(filteredSeq, useList);
    delete filteredSeq;
    return result;
}

template <class T>
T Stack<T>::Reduce(T (*func)(T, T), T initial) const {
    return seq->Reduce(func, initial);
}

template <class T>
Stack<T>* Stack<T>::Concat(const Stack<T>& other) const {
    Sequence<T>* concatenatedSeq = seq->Concat(other.seq);
    Stack<T>* result = new Stack<T>(concatenatedSeq, useList);
    delete concatenatedSeq;
    return result;
}

template <class T>
Stack<T>* Stack<T>::GetSubsequence(size_t startIndex, size_t endIndex) const {
    Sequence<T>* subSeq = seq->GetSubsequence(startIndex, endIndex);
    Stack<T>* result = new Stack<T>(subSeq, useList);
    delete subSeq;
    return result;
}

template <class T>
int Stack<T>::FindSubsequence(const Sequence<T>* sub) const {
    if (sub == nullptr) {
        throw InvalidArgument("Null subsequence");
    }

    size_t subLen = sub->GetCount();
    size_t mainLen = seq->GetCount();

    if (subLen == 0) {
        return 0;
    }
    if (subLen > mainLen) {
        return -1;
    }

    for (size_t i = 0; i <= mainLen - subLen; ++i) {
        bool match = true;
        for (size_t j = 0; j < subLen; ++j) {
            if (!(seq->Get(i + j) == sub->Get(j))) {
                match = false;
                break;
            }
        }
        if (match) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

template <class T>
int Stack<T>::FindSubsequence(const Stack<T>& sub) const {
    return FindSubsequence(sub.seq);
}

template <class T>
bool Stack<T>::ContainsSubsequence(const Sequence<T>* sub) const {
    return FindSubsequence(sub) != -1;
}

template <class T>
bool Stack<T>::ContainsSubsequence(const Stack<T>& sub) const {
    return FindSubsequence(sub.seq) != -1;
}

template <class T>
Stack<T> Stack<T>::operator+(const Stack<T>& other) const {
    Stack<T>* temp = this->Concat(other);
    Stack<T> result(*temp);
    delete temp;
    return result;
}

template <class T>
Stack<T>& Stack<T>::operator<<(const T& item) {
    this->Push(item);
    return *this;
}

template <class T>
bool Stack<T>::operator==(const Stack<T>& other) const {
    if (this->GetCount() != other.GetCount()) {
        return false;
    }
    for (size_t i = 0; i < this->GetCount(); ++i) {
        if (!(this->Get(i) == other.Get(i))) {
            return false;
        }
    }
    return true;
}

template <class T>
bool Stack<T>::operator!=(const Stack<T>& other) const {
    return !(*this == other);
}

template <class T>
T Stack<T>::operator[](size_t index) const {
    return seq->Get(index);
}

template <class T>
IEnumerator<T>* Stack<T>::GetEnumerator() const {
    return seq->GetEnumerator();
}