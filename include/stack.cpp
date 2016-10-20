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


template<typename T>
T *new_with_copy(const T *tmp, size_t count, size_t array_size) {  /* strong */
    T *array_ = new T[array_size];
    try { std::copy(tmp, tmp + count, array_); }
    catch (...) {
        delete[] array_;
        throw;
    }
    return array_;
}

template<typename T>
class Allocator {
protected:
    Allocator(size_t size = 0);

    virtual ~Allocator();

    auto swap(Allocator &other) -> void;

    Allocator(Allocator const &) = delete;

    auto operator=(Allocator const &) -> Allocator & = delete;

    T *ptr_;
    size_t size_;
    size_t count_;
};

template<typename T>
Allocator<T>::Allocator(size_t size)
        :    size_(size),
            count_(0)
{
    ptr_ = new T[size];
}

template<typename T>
Allocator<T>::~Allocator() {
    if (count_ != 0) {
        delete[] ptr_;
    }
}

template<typename T>
auto Allocator<T>::swap(Allocator<T> &other) -> void {
    std::swap(ptr_, other.ptr_);
    std::swap(count_, other.count_);
    std::swap(size_, other.size_);
}


template<typename T>
class Stack : public Allocator<T> {
public:
    Stack();    /* noexcept*/

    ~Stack();   /* noexcept */

    Stack(const Stack &);   /* strong */

    Stack &operator=(const Stack &);    /* strong */

    size_t count() const;  /* noexcept */

    void push(T const &);   /* strong */

    void pop();     /* strong */

    const T& top();    /* strong */



    bool empty() const; /* noexcept */
private:
    void grow(); /* strong */
};

template<typename T>
Stack<T>::Stack()
{ }

template<typename T>
Stack<T>::~Stack() {
}

template<typename T>
size_t Stack<T>::count() const {
    return Allocator<T>::count_;
}

template<typename T>
void Stack<T>::push(T const &element) {
    if (Allocator<T>::size_ == Allocator<T>::count_) {
        grow();
    }
    Allocator<T>::ptr_[Allocator<T>::count_] = element;
    Allocator<T>::count_++;
}

template<typename T>
void Stack<T>::grow() {
    size_t new_array_size_ = max(1, Allocator<T>::size_ * 2);
    T *new_array_ = new_with_copy(Allocator<T>::ptr_, Allocator<T>::count_, new_array_size_);
    if (Allocator<T>::count_ != 0) {
        delete[] Allocator<T>::ptr_;
    }
    Allocator<T>::ptr_ = new_array_;
    Allocator<T>::size_ = new_array_size_;
}

template<typename T>
void Stack<T>::pop() {
    if (empty()) {
        throw std::logic_error("Stack is empty!");
    }   
    else { 
        Allocator<T>::count_--;    
    }
}

template<typename T>
Stack<T>::Stack(const Stack &tmp) {
    Allocator<T>::ptr_ = new_with_copy(tmp.ptr_, tmp.count_, tmp.size_);
    Allocator<T>::count_ = tmp.count_;
    Allocator<T>::size_ = tmp.size_;
}

template<typename T>
Stack<T> &Stack<T>::operator=(const Stack<T> &tmp) {
    if (this != &tmp) {
        if (!empty()) {
            delete[] Allocator<T>::ptr_;
        }
        Allocator<T>::ptr_ = new_with_copy(tmp.ptr_, tmp.count_, tmp.size_);
        Allocator<T>::count_ = tmp.count_;
        Allocator<T>::size_ = tmp.size_;
    }
    return *this;
}

template<typename T>
const T& Stack<T>::top() {
    if (empty()) {
        throw std::logic_error("Stack is empty!");
    }
    else {
       return Allocator<T>::ptr_[Allocator<T>::count_ - 1];
    }
}

template<typename T>
bool Stack<T>::empty() const {
    return Allocator<T>::count_ == 0;
}

#endif
