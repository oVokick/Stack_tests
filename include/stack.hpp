#ifndef stack_hpp
#define stack_hpp
#pragma once

#include <iostream>
#include <new>
#include <stdexcept>
#include <vector>
#include <memory>

size_t max(size_t a, size_t b) {
	return a > b ? a : b;
}

class bitset {
public:
	explicit bitset(size_t size) /*strong*/;

	bitset(bitset const &other) = delete;

	auto operator=(bitset const &other)->bitset & = delete;

	bitset(bitset &&other) = delete;

	auto operator=(bitset &&other)->bitset & = delete;

	auto set(size_t index) /*strong*/ -> void;

	auto reset(size_t index) /*strong*/ -> void;

	auto test(size_t index) /*strong*/ -> bool;

	auto size() /*noexcept*/ -> size_t;

	auto counter() /*noexcept*/ -> size_t;

private:
	std::unique_ptr<bool[]> ptr_;
	size_t size_;
	size_t counter_;
};

bitset::bitset(size_t size)
	: ptr_(std::make_unique<bool[]>(size)),
	size_(size),
	counter_(0)
{
	;
}

auto bitset::size() -> size_t {
	return size_;
}

auto bitset::counter() -> size_t {
	return counter_;
}

auto bitset::set(size_t index) -> void {
	if (index >= size_){
		std::out_of_range("Error");
	}
	if (!test(index)){
		ptr_[index] = true;
		++counter_;
	}
}

auto bitset::reset(size_t index) -> void {
	if (index >= size_){
		std::out_of_range("Error");
	}
	if (test(index)) {
		ptr_[index] = false;
		--counter_;
	}
}

auto bitset::test(size_t index) -> bool {
	if (index >= size_){
		std::out_of_range("Error");
	}
	return ptr_[index];
}


template<typename T>
class Allocator {
protected:
public:
	explicit
		Allocator(std::size_t size = 0) /*strong*/;
	Allocator(Allocator const & other) /*strong*/;
	auto operator =(Allocator const & other)->Allocator & = delete;
	~Allocator();

	auto resize() /*strong*/ -> void;

	auto construct(T * ptr, T const & value) /*strong*/ -> void;
	auto destroy(T * ptr) /*noexcept*/ -> void;

	auto get() /*noexcept*/ -> T *;
	auto get() const /*noexcept*/ -> T const *;

	auto count() const /*noexcept*/ -> size_t;
	auto full() const /*noexcept*/ -> bool;
	auto empty() const /*noexcept*/ -> bool;
	auto swap(Allocator &other) /*noexcept*/ -> void;
private:
	auto destroy(T * first, T * last) /*noexcept*/ -> void;



	T *ptr_;
	size_t size_;
	std::unique_ptr<bitset> map_;
};

template<typename T>
Allocator<T>::Allocator(size_t size)
	: ptr_(static_cast<T *>(size == 0 ? nullptr : operator new(size * sizeof(T)))),
	size_(size),
	map_(std::make_unique<bitset>(size))
{
	;
}

template <typename T>
Allocator<T>::Allocator(Allocator const &other)
	: Allocator<T>(other.size_)

{
	for (int i = 0; i < size_; ++i) {
		if (map_->test(i)) {
			destroy(ptr_ + i);
		}
		construct(ptr_ + i, other.ptr_[i]);
	}
}

template<typename T>
Allocator<T>::~Allocator() {
	if (map_->counter() > 0) {
		destroy(ptr_, ptr_ + size_);
	}
	operator delete(ptr_);
}

template<typename T>
auto Allocator<T>::swap(Allocator<T> &other) -> void {
	std::swap(map_, other.map_);
	std::swap(ptr_, other.ptr_);
	std::swap(size_, other.size_);
}

template<typename T>
void Allocator<T>::construct(T *ptr, T const &value) {
	if (ptr < ptr_ || ptr >= ptr_ + size_) {
		std::out_of_range("Error");
	}
	new(ptr)T(value);
	map_->set(ptr - ptr_);
}

template<typename T>
void Allocator<T>::destroy(T *ptr) {
	ptr->~T();
	map_->reset(ptr - ptr_);
}

template<typename T>
auto Allocator<T>::resize() -> void {
	size_t new_size = max(size_ * 2, 1);
	Allocator<T> buff(new_size);
	for (size_t i = 0; i < size_; ++i) {
		if (map_->test(i))
		{
			buff.construct(buff.ptr_ + i, ptr_[i]);
		}
	}
	this->swap(buff);
}

template<typename T>
auto Allocator<T>::get() -> T * {
	return ptr_;
}

template<typename T>
auto Allocator<T>::get() const -> const T *{
	return ptr_;
}

template<typename T>
auto Allocator<T>::count() const -> size_t {
	return map_->counter();
}

template<typename T>
auto Allocator<T>::full() const -> bool {
	return map_->counter() == size_;
}

template<typename T>
auto Allocator<T>::empty() const -> bool {
	return map_->counter() == 0;
}

template<typename T>
auto Allocator<T>::destroy(T *first, T *last) -> void {
	for (; first != last; ++first) {
		destroy(&*first);
	}
}



template<typename T>
class Stack{
public:
	explicit
		Stack(size_t size = 0);
	Stack(Stack const &other) = default;
	auto operator =(Stack const &other) /*strong*/ -> Stack &;

	auto empty() const /*noexcept*/ -> bool;
	auto count() const /*noexcept*/ -> size_t;

	auto push(T const & value) /*strong*/ -> void;
	auto pop()  -> void; /*strong*/
	auto top()->T &; /*strong*/
	auto top() const->T const &; /*strong*/
private:
	Allocator<T> allocator_;

	auto throw_is_empty() const -> void;
};

template<typename T>
Stack<T>::Stack(size_t size)
	: allocator_(size) {
	;
}

template<typename T>
auto Stack<T>::count() const -> size_t {
	return allocator_.count();
}

template<typename T>
auto Stack<T>::push(const T &value) -> void {
	if (allocator_.full()) {
		allocator_.resize();
	}
	allocator_.construct(allocator_.get() + allocator_.count(), value);
}

template<typename T>
auto Stack<T>::pop() -> void {
	throw_is_empty();
	allocator_.destroy(allocator_.get() + (this->count() - 1));
}

template<typename T>
auto Stack<T>::operator=(const Stack<T> &tmp) -> Stack & {
	if (this != &tmp) {
		Stack(tmp).allocator_.swap(allocator_);
	}
	return *this;
}

template<typename T>
auto Stack<T>::top() const -> T const & {
	throw_is_empty();
	return(*(allocator_.get() + this->count() - 1));
}

template<typename T>
auto Stack<T>::top() -> T & {
	throw_is_empty();
	return(*(allocator_.get() + this->count() - 1));
}

template<typename T>
auto Stack<T>::empty() const -> bool {
	return count() == 0;
}

template <typename T>
auto Stack<T>::throw_is_empty() const -> void {
	if (empty()) {
		std::logic_error("stack is empty");
	}
}

#endif
