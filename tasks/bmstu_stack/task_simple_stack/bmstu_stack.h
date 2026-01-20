#pragma once

#include <exception>
#include <iostream>
#include <utility>
#include <stdexcept>

namespace bmstu
{
template <typename T>
class stack
{
   public:
	stack() : data_(nullptr), size_(0u) {}

	bool empty() const noexcept { return size_ == 0u; }

	size_t size() const noexcept { return size_; }

	~stack() {
        clear();
        operator delete(data_);
    }
    stack(const stack& other) : data_(nullptr), size_(0u)
    {
        data_ = (T*)operator new(other.size_ * sizeof(T));
        for (size_t i = 0; i < other.size_; ++i) {
            new(data_ + i) T(other.data_[i]);
        }
        size_ = other.size_;
	}

    stack(stack&& other) noexcept : data_(other.data_), size_(other.size_)
	{
	    other.data_ = nullptr;
	    other.size_ = 0u;
	}

    stack& operator=(const stack& other)
	{
        if (this != &other) {
            clear();
            operator delete(data_);
            data_ = nullptr;
            size_ = 0u;
            for (size_t i = 0; i < other.size_; ++i) {
                new(data_ + i) T(other.data_[i]);
            }
            size_ = other.size_;
        }
	    return *this;
	}
    stack& operator=(stack&& other) noexcept
	{
        if (this != &other) {
            clear();
            operator delete(data_);
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0u;
        }
	    return *this;
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
        grow();
        new (data_ + size_) T(std::forward<Args>(args)...);
        ++size_;

	}

	void push(T&& value) {
        grow();
        new(data_ + size_) T(std::move(value));
        ++size_;
    }

	void clear() noexcept {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }

	void push(const T& value) {
        grow();
        new(data_ + size_) T(value);
        ++size_;
    }

	void pop() {
        if (empty()) { throw std::underflow_error("stack is empty"); }
        data_[size_-1].~T();
        --size_;
    }

	T& top() { 
        if (empty()) {
            throw std::underflow_error("stack is empty");
        }
        return data_[size_ - 1];
    }

	const T& top() const {
        if (empty()) {
            throw std::underflow_error("stack is empty");
        }
        return data_[size_ - 1];
    }

   private:

   void grow() {
    const size_t new_size = size_ + 1;
    T* new_data = (T*)operator new(new_size * sizeof(T));
    for (size_t i = 0; i < size_; ++i) {
        new (new_data + i) T(std::move(data_[i]));
        data_[i].~T();
    }
    operator delete(data_);
    data_ = new_data;
   }
	T* data_;
	size_t size_;
};
}  // namespace bmstu


template<typename T>
T summmmm(T a, T b) {
    return a + b; 
}

int main() {
    std::cout << summmmm(3.24, 4.45) << std::endl;
}