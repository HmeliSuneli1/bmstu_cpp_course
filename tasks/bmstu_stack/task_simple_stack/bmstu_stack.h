#pragma once

#include <exception>
#include <iostream>
#include <new>
#include <utility>
namespace bmstu
{
template <typename T>
class stack
{
   public:
	stack() noexcept : data_(nullptr), size_(0u) {}

	bool empty() const noexcept { return size_ == 0u; }

	size_t size() const noexcept { return size_; }

	~stack()
	{
		clear();
		::operator delete[](data_);
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
		grow();
		new (data_ + size_) T(std::forward<Args>(args)...);
		++size_;
	}

	void push(const T& value)
	{
		grow();
		new (data_ + size_) T(value);
		++size_;
	}

	void push(T&& value)
	{
		grow();
		new (data_ + size_) T(std::move(value));
		++size_;
	}

	void pop()
	{
		if (empty())
		{
			throw std::underflow_error("stack is empty");
		}
		data_[size_ - 1].~T();
		--size_;
	}

	T& top()
	{
		if (empty())
		{
			throw std::underflow_error("stack is empty");
		}
		return data_[size_ - 1];
	}

	const T& top() const
	{
		if (empty())
		{
			throw std::underflow_error("stack is empty");
		}
		return data_[size_ - 1];
	}

	void clear() noexcept
	{
		for (size_t i = 0; i < size_; ++i)
		{
			data_[i].~T();
		}
		size_ = 0;
	}

   private:
	void grow()
	{
		const size_t new_cap = size_ + 1;
		T* new_data = (T*)::operator new[](new_cap * sizeof(T));

		for (size_t i = 0; i < size_; ++i)
		{
			new (new_data + i) T(std::move(data_[i]));
			data_[i].~T();
		}

		::operator delete[](data_);
		data_ = new_data;
	}

	T* data_;
	size_t size_;
};
}  // namespace bmstu