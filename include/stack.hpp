#ifndef stack_cpp
#define stack_cpp
#pragma once

#include <iostream>
#include <stdexcept>

using namespace std;

size_t max(size_t a, size_t b) {
    return a > b ? a : b;
}

template<typename T1, typename T2>
void construct(T1 *ptr, T2 const &value) {
    new(ptr) T1(value);
}

template<typename T>
void destroy(T *ptr) noexcept {
    ptr->~T();
}

template<typename FwdIter>
void destroy(FwdIter first, FwdIter last) noexcept {
    for (; first != last; ++first) {
        destroy(&*first);
    }
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

    ~Allocator();

    auto swap(Allocator &other) -> void;

    Allocator(Allocator const &) = delete;

    auto operator=(Allocator const &) -> Allocator & = delete;

    T *ptr_;
    size_t size_;
    size_t count_;
};

template<typename T>
Allocator<T>::Allocator(size_t size)
        :   ptr_(new T[size]),
            size_(size),
            count_(0)
{}

template<typename T>
Allocator<T>::~Allocator() {
    operator delete(ptr_);
}

template<typename T>
auto Allocator<T>::swap(Allocator<T> &other) -> void {
    std::swap(ptr_, other.ptr_);
    std::swap(count_, other.count_);
    std::swap(size_, other.size_);
}


template<typename T>
class Stack : private Allocator<T> {
public:
    Stack(size_t size = 0);  /* noexcept*/

    ~Stack();   /* noexcept */

    Stack(const Stack &);   /* strong */

    auto operator=(const Stack &) ->Stack &;    /* strong */

    auto count() const -> size_t;   /* noexcept */

    auto push(const T &value) -> void;  /* strong */

    auto pop() -> void;     /* strong */

    auto top() -> const T&;    /* strong */



    auto empty() const -> bool; /* noexcept */
private:
    auto grow() -> void; /* strong */
};

template<typename T>
Stack<T>::Stack(size_t size) {
    this->ptr_ = new T[size];
    this->count_ = 0;
    this->size_ = size;
}

template<typename T>
Stack<T>::Stack(const Stack &tmp) {
    this->ptr_ = new_with_copy(tmp.ptr_, tmp.count_, tmp.size_);
    this->count_ = tmp.count_;
    this->size_ = tmp.size_;
}

template<typename T>
Stack<T>::~Stack() {
}

template<typename T>
auto Stack<T>::count() const ->size_t {
    return this->count_;
}

template<typename T>
auto Stack<T>::push(const T &value) -> void {
    if (this->count_ == this->size_) {
        grow();
    }

    construct(this->ptr_ + this->count_, value);
    ++this->count_;
}


template<typename T>
auto Stack<T>::grow() ->void {
    size_t array_size = max(this->size_ * 2, 0);

    Stack temp{array_size};
    while (temp.count() < this->count_) {
        temp.push(this->ptr_[temp.count()]);
    }

    this->swap(temp);
}

template<typename T>
auto Stack<T>::pop() -> void {
    if (empty()) {
        throw std::logic_error("Stack is empty!");
    }
    this->count_--;
}

template<typename T>
auto Stack<T>::operator=(const Stack<T> &tmp) ->Stack & {
    if (this != &tmp) {
        if (!empty()) {
            delete[] this->ptr_;
        }
        this->ptr_ = new_with_copy(tmp.ptr_, tmp.count_, tmp.size_);
        this->count_ = tmp.count_;
        this->size_ = tmp.size_;
    }
    return *this;
}

template<typename T>
auto Stack<T>::top() ->const T& {
    if (empty()) {
        throw std::logic_error("Stack is empty!");
    }
    return this->ptr_[this->count_ - 1];
}

template<typename T>
auto Stack<T>::empty() const ->bool {
    return this->count_ == 0;
}

#endif
