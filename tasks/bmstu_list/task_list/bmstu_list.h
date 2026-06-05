#include <cstddef>
#include <iterator>
#include <ostream>
#include <compare>
#include <initializer_list>
#include <utility>
#include "abstract_iterator.h"

namespace bmstu
{
template <typename T>
class list
{
	struct node
	{
		node() = default;

		node(node* prev, const T& value, node* next)
			: value_(value), next_node_(next), prev_node_(prev)
		{
		}

		T value_;
		node* next_node_ = nullptr;
		node* prev_node_ = nullptr;
	};

   public:
	struct iterator
		: public abstract_iterator<iterator, T, std::bidirectional_iterator_tag>
	{
		using base_type = abstract_iterator<iterator, T, std::bidirectional_iterator_tag>;
		using difference_type = typename base_type::difference_type;
		using reference = typename base_type::reference;
		using pointer = typename base_type::pointer;
		node* current;
		iterator() : current(nullptr) {}
		iterator(node* node) : current(node) {}
		iterator& operator++() override { 
			if (current) current = current->next_node_;
			return *this; }
		iterator& operator--() override { 
			if (current) current = current->prev_node_;
			return *this; }
		iterator operator++(int) override {
			iterator tmp =  *this;
			++(*this);
			return tmp;
		}
		iterator operator--(int) override { 
			iterator tmp = *this;
			--(*this);
			return tmp; 
		}
		iterator& operator+=( const typename abstract_iterator<iterator,T,std::bidirectional_iterator_tag>::difference_type& n)
		{
			difference_type steps = n;
			if (steps > 0) {
				while (steps-- > 0 && current) {
					current = current->next_node_;
				}
			} else {
				while (steps++ < 0 && current) {
					current = current->prev_node_;
				}
			}
			return *this;
		}
		iterator& operator-=(
			const typename abstract_iterator<
				iterator,
				T,
				std::bidirectional_iterator_tag>::difference_type& n)
		{
			return *this += (-n);
		}
		iterator operator+(const typename abstract_iterator<
						   iterator,
						   T,
						   std::bidirectional_iterator_tag>::difference_type& n)
			const
		{
			iterator tmp = *this;
			return tmp += n;
		}
		iterator operator-(const typename abstract_iterator<
						   iterator,
						   T,
						   std::bidirectional_iterator_tag>::difference_type& n)
			const
		{
			iterator tmp = *this;
			return tmp -= n;
		}
		typename abstract_iterator<iterator,
								   T,
								   std::bidirectional_iterator_tag>::reference
		operator*() const override
		{
			return current->value_;
		}
		typename abstract_iterator<iterator,
								   T,
								   std::bidirectional_iterator_tag>::pointer
		operator->() const override
		{
			return &(current->value_);
		}
		bool operator==(const iterator& other) const override
		{
			return current == other.current;
		}
		bool operator!=(const iterator& other) const override
		{
			return current != other.current;
		}

		explicit operator bool() const override { return current != nullptr; }
		typename abstract_iterator<
			iterator,
			T,
			std::bidirectional_iterator_tag>::difference_type
		operator-(const iterator& other) const override
		{
			difference_type dist = 0;
			node* curr = other.current;
			while (curr != current && curr != nullptr) {
				++dist;
				curr = curr->next_node_;
			}
			if (curr == current) {
				return dist;
			}
			dist = 0;
			curr = current;
			while (curr != other.current && curr != nullptr) {
				--dist;
				curr = curr->next_node_;
		}
		return dist;
		}
	};
	using const_iterator = iterator;

	list() {
		head_ = new node();
	    tail_ = new node();
	    head_->next_node_ = tail_;
	    tail_->prev_node_ = head_;
	    size_ = 0;
	}
	template <typename it>
	list(it begin, it end) : list()
	{
		for (auto current = begin; current != end; ++current) {
			push_back(*current);
		}
	}

	list(std::initializer_list<T> values) : list() {

		for (const auto& value : values) {
			push_back(value);
		}
	}

	list(const list& other) : list() {
		for (const auto& value : other) {
			push_back(value);
		}
	}

	list(list&& other) : list() {
		swap(other);
	}

    list& operator=(const list& other)
	{
	    if (this != &other)
	    {
	        list tmp(other);
	        swap(tmp);
	    }
	    return *this;
	}
    list& operator=(list&& other) noexcept {
	    swap(other);
	    return *this;
	}

#pragma endregion
#pragma region pushs

	template <typename Type>
	void push_back(const Type& value)
	{
		node* last = tail_->prev_node_;
		node* new_last = new node(tail_->prev_node_, value, tail_);
		tail_->prev_node_ = new_last;
		last->next_node_ = new_last;
		++size_;
	}

	template <typename Type>
	void push_front(const Type& value)
	{
		// адрес реального последнего элемента
		node* first = head_->next_node_;
		node* new_first = new node(head_, value, first);
		head_->next_node_ = new_first;
		first->prev_node_ = new_first;
		++size_;
	}

#pragma endregion

	bool empty() const

		noexcept
	{
		return (size_ == 0u);
	}

	~list()
{
    clear();
    delete head_;
    delete tail_;
}

	void clear()
{
    node* curr = head_->next_node_;
    while (curr != tail_)
    {
        node* next = curr->next_node_;
        delete curr;
        curr = next;
    }
    head_->next_node_ = tail_;
    tail_->prev_node_ = head_;
    size_ = 0;
}

	size_t size() const { return size_; }

	void swap(list& other)
		noexcept
	{
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
	}

	friend void swap(list& l, list& r) noexcept { l.swap(r); }

#pragma region iterators

	iterator begin()

		noexcept
	{
		return iterator{head_->next_node_};
	}

	iterator end()

		noexcept
	{
		return iterator{tail_};
	}

	const_iterator begin() const

		noexcept
	{
		return const_iterator{head_->next_node_};
	}

	const_iterator end() const

		noexcept
	{
		return const_iterator{tail_};
	}

	const_iterator cbegin() const

		noexcept
	{
		return const_iterator{head_->next_node_};
	}

	const_iterator cend() const

		noexcept
	{
		return const_iterator{tail_};
	}
	const_iterator find(const T& v ) const {
		auto it = begin();
		while (it != end()) {
			if (*it == v) {
				return it;
			}
			it++;
		}
		return end();
	}

#pragma endregion

	T operator[](size_t pos) const
	{
	    node* curr = head_->next_node_;
	    for (size_t i = 0; i < pos; i++)
	    {
	        curr = curr->next_node_;
	    }
	    return curr->value_;
	}

	T& operator[](size_t pos)
	{
	    node* curr = head_->next_node_;
	    for (size_t i = 0; i < pos; i++)
	        curr = curr->next_node_;
	    return curr->value_;
	}

	friend bool operator==(const list& l, const list& r)
{
    if (l.size() != r.size()) return false;
    auto it1 = l.begin();
    auto it2 = r.begin();
    while (it1 != l.end())
    {
        if (*it1 != *it2) return false;
        ++it1;
        ++it2;
    }
    return true;
	}

	friend bool operator!=(const list& l, const list& r) { return !(l == r); }

    friend auto operator<=>(const list& lhs, const list& rhs)
	{
	    auto it1 = lhs.begin();
	    auto it2 = rhs.begin();
	    while (it1 != lhs.end() && it2 != rhs.end())
	    {
	        if (auto cmp = *it1 <=> *it2; cmp != 0) {
	            return cmp;
	        }
	        ++it1;
	        ++it2;
	    }
	    return lhs.size() <=> rhs.size();
	}
	friend std::ostream& operator<<(std::ostream& os, const list& other)
	{
	    os << "{";
	    auto it = other.begin();
	    if (it != other.end())
	    {
	        os << *it;
	        ++it;
	    }
	    for (; it != other.end(); ++it)
	    {
	        os << ", " << *it;
	    }
	    os << "}";
		return os;
	}

	iterator insert(const_iterator pos, const T& value)
	{
	    node* current_node = pos.current;
	    node* new_node = new node(current_node->prev_node_, value, current_node);
	    current_node->prev_node_->next_node_ = new_node;
	    current_node->prev_node_ = new_node;
	    ++size_;
		return iterator{new_node};
	}

   private:
	static bool lexicographical_compare_(const list<T>& l, const list<T>& r)
	{
		auto it1 = l.begin();
	    auto it2 = r.begin();
	    while (it1 != l.end() && it2 != r.end())
	    {
	        if (*it1 < *it2) return true;
	        if (*it1 > *it2) return false;
	        ++it1;
	        ++it2;
	    }
	    return (it1 == l.end()) && (it2 != r.end());
	}

	size_t size_ = 0;
	node* tail_ = nullptr;
	node* head_ = nullptr;
};
}  // namespace bmstu