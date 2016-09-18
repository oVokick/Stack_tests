#ifndef stack_cpp
#define stack_cpp
#pragma once

#ifndef stack_cpp
#define stack_cpp
#pragma once
#include <iostream>
using namespace std

size_t max(size_t a, size_t b) {
    return a > b ? a : b;
}

template<typename T>
class Stack {
public:
    Stack();

    ~Stack();

    Stack(const Stack &);

    Stack& operator =(const Stack &);

    size_t count() const noexcept;

    void push(T const &);

    T pop();

    bool is_empty() const;

private:
    void new_with_empty(const T*);
    void grow();

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
    if (!is_empty()) {
        delete[] array_;
    }
}

template<typename T>
size_t Stack<T>::count() const noexcept {
    return count_;
}

template<typename T>
void Stack<T>::push(T const &element) {
    if (array_size_ <= count_) {
        grow();
    }
   array_[count_] = element;
   count_++;
}

template<typename T>
void Stack<T>::grow() {
    size_t new_array_size_ = max(1, array_size_ * 2);
    T *new_array_ = new T[new_array_size_];
    for (int i = 0; i < count_; i++) {
        new_array_[i] = array_[i];
    }
    if (!is_empty()) {
        delete[] array_;
    }
    array_ = new_array_;
    array_size_ = new_array_size_;
    return;
}

template<typename T>
T Stack<T>::pop() {
    if (is_empty()) {
        throw std::logic_error("Stack is empty!");
    }
    --count_;
    return array_[count_];
}

template<typename T>
bool Stack<T>::is_empty() const {
    return count_ == 0;
}

template <typename T>
void Stack<T>::new_with_empty(const T *tmp) {
    array_ = new T[array_size_];
    copy(tmp, tmp + count_, array_);
}

template <typename T>
Stack<T>::Stack(const Stack &tmp)
        :   count_(tmp.count_),
            array_size_(tmp.array_size_)
{
    new_with_empty(tmp.array_);
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T> &tmp) {
    count_ = tmp.count_;
    array_size_ = tmp.array_size_;
    new_with_empty(tmp.array_);
    return *this;
}

#endif
