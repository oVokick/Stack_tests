#ifndef stack_cpp
#define stack_cpp
#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdexcept>
using namespace std;

size_t max(size_t a, size_t b) {
    return a > b ? a : b;
}

template <typename T>
T* new_with_copy(const T *tmp, size_t count, size_t array_size) {  /* strong */
    T *array_ = new T[array_size];
    try{ copy(tmp, tmp + count, array_); }
    catch (...){ delete[] array_; throw; }
    return array_;
}

template<typename T>
class Stack {
public:
    Stack(); /* noexcept*/

    ~Stack(); /* noexcept */

    Stack(const Stack &); /* strong */

    Stack& operator = (const Stack &); /* strong */

    size_t count() const; /* noexcept */

    void push(T const &); /* strong */

    void pop();  /* strong */

    T& top() const; /* strong */

private:
    void grow(); /* strong */

    T *array_;
    size_t array_size_;
    size_t count_;
};

template<typename T>
Stack<T>::Stack()
        : array_size_(0),
          count_(0) { }

template<typename T>
Stack<T>::~Stack() {
    if (count_ == 0) {
        delete[] array_;
    }
}

template<typename T>
size_t Stack<T>::count() const {
    return count_;
}

template<typename T>
void Stack<T>::push(T const &element) {
    if (array_size_ == count_) {
        grow();
    }
    array_[count_] = element;
    count_++;
}

template<typename T>
void Stack<T>::grow() {
    size_t new_array_size_ = max(1, array_size_ * 2);
    T *new_array_ = new_with_copy(array_, count_, new_array_size_);
    if (count_ != 0) {
        delete[] array_;
    }
    array_ = new_array_;
    array_size_ = new_array_size_;
    return;
}

template<typename T>
void Stack<T>::pop() {
    if (count_ == 0) {
        throw std::logic_error("Stack is empty!");
    }
    --count_;
}

template <typename T>
T& Stack<T>::top() const {
    if (count_ == 0) {
        throw std::logic_error("Stack is empty!");
    }
    return array_[count_ - 1];
}

template <typename T>
Stack<T>::Stack(const Stack &tmp)
        :   count_(tmp.count_),
            array_size_(tmp.array_size_)
{
    array_ = new_with_copy(tmp.array_, count_, array_size_);
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T> &tmp) {
    if (this != &tmp) {
        count_ = tmp.count_;
        array_size_ = tmp.array_size_;
        delete[] array_;
        array_ = new_with_copy(tmp.array_, count_, array_size_);
    }
    return *this;
}

#endif
