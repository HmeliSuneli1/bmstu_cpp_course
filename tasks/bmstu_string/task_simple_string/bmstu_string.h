#pragma once

#include <exception>
#include <iostream>

namespace bmstu
{
    template <typename T> class simple_basic_string;

    typedef simple_basic_string<char> string;
    typedef simple_basic_string<wchar_t> wstring;
    typedef simple_basic_string<char16_t> u16string;
    typedef simple_basic_string<char32_t> u32string;

    template <typename T> class simple_basic_string
    {
      public:
        /// Конструктор по умолчанию
        simple_basic_string() : ptr_(new T[1]{0}), size_(0) {}

        simple_basic_string(size_t size) : ptr_(new T[size + 1]), size_(size)
        {
            for (size_t i = 0; i < size_; i++)
            {
                ptr_[i] = ' ';
            }
            ptr_[size_] = '\0';
        }

        simple_basic_string(std::initializer_list<T> il)
            : ptr_(new T[il.size() + 1]), size_(il.size())
        {
            for (size_t i = 0; i < size_; i++)
            {
                ptr_[i] = *(il.begin() + i);
            }
            ptr_[size_] = '\0';
        }

        /// Конструктор с параметром си-с
        simple_basic_string(const T* c_str)
        {
            size_ = strlen_(c_str);
            ptr_ = new T[size_ + 1];
            for (size_t i = 0; i < size_; i++)
            {
                ptr_[i] = c_str[i];
            }
            ptr_[size_] = '\0';
        }

        /// Конструктор копирования
        simple_basic_string(const simple_basic_string& other)
        {
            size_ = other.size_;
            ptr_ = new T[size_ + 1];
            for (size_t i = 0; i < size_; i++)
            {
                ptr_[i] = other.ptr_[i];
            }
            ptr_[size_] = '\0';
        }

        /// Перемещающий конструктор
        simple_basic_string(simple_basic_string&& dying) : size_(dying.size_), ptr_(dying.ptr_)
        {
            dying.ptr_ = new T[1]{'\0'};
            dying.size_ = 0;
        }

        /// Деструктор
        ~simple_basic_string()
        {
            delete[] ptr_;
        }

        /// Геттер на си-строку
        const T* c_str() const
        {
            return ptr_;
        }

        size_t size() const
        {
            return size_;
        }

        /// Оператор копирующего присваивания
        simple_basic_string& operator=(simple_basic_string&& other)
        {
            if (this == &other)
            {
                return *this;
            }
            delete[] ptr_;
            ptr_ = other.ptr_;
            size_ = other.size_;
            other.ptr_ = new T[1]{'\0'};
            other.size_ = 0;
            return *this;
        }

        /// Оператор копирующего присваивания си строки
        simple_basic_string& operator=(const T* c_str)
        {
            delete[] ptr_;
            size_ = strlen_(c_str);
            T* new_ptr = new T[size_ + 1];
            for (size_t i = 0; i < size_; i++)
            {
                new_ptr[i] = c_str[i];
            }
            new_ptr[size_] = '\0';
            ptr_ = new_ptr;
            return *this;
        }

        /// Оператор копирующего присваивания
        simple_basic_string& operator=(const simple_basic_string& other)
        {
            if (this == &other)
            {
                return *this;
            }
            delete[] ptr_;
            size_ = other.size_;
            T* new_ptr_ = new T[size_ + 1];
            for (size_t i = 0; i < size_; i++)
            {
                new_ptr_[i] = other.ptr_[i];
            }
            new_ptr_[size_] = '\0';
            ptr_ = new_ptr_;
            return *this;
        }

        friend simple_basic_string<T> operator+(const simple_basic_string<T>& left,
                                                const simple_basic_string<T>& right)
        {
            simple_basic_string new_string(left.size_ + right.size_);
            for (size_t i = 0; i < left.size_; i++)
            {
                new_string.ptr_[i] = left.ptr_[i];
            }
            for (size_t i = 0; i < right.size_; i++)
            {
                new_string.ptr_[i + left.size_] = right.ptr_[i];
            }
            new_string.ptr_[new_string.size_] = '\0';
            return new_string;
        }

        template <typename S> friend S& operator<<(S& os, const simple_basic_string& obj)
        {
            os << obj.c_str();
            return os;
        }

        template <typename S> friend S& operator>>(S& is, simple_basic_string& obj)
        {
            obj.clean_();
            obj.ptr_ = new T[1]{'\0'};
            T character;
            while (is.get(character))
            {
                obj += character;
            }
            return is;
        }

        simple_basic_string& operator+=(const simple_basic_string& other)
        {
            T* new_ptr = new T[size_ + other.size_ + 1];
            for (size_t i = 0; i < size_; i++)
            {
                new_ptr[i] = ptr_[i];
            }
            for (size_t i = 0; i < other.size_; i++)
            {
                new_ptr[i + size_] = other.ptr_[i];
            }
            new_ptr[size_ + other.size_] = '\0';
            delete[] ptr_;
            ptr_ = new_ptr;
            size_ += other.size_;
            return *this;
        }

        simple_basic_string& operator+=(T symbol)
        {
            T* new_ptr = new T[size_ + 2];
            for (size_t i = 0; i < size_; i++)
            {
                new_ptr[i] = ptr_[i];
            }
            new_ptr[size_] = symbol;
            new_ptr[size_ + 1] = '\0';
            delete[] ptr_;
            ptr_ = new_ptr;
            size_ += 1;
            return *this;
        }

        T& operator[](size_t index) noexcept
        {
            return *(ptr_ + index);
        }

        T& at(size_t index)
        {
            if (index < 0 || index >= size_)
                throw std::out_of_range("Wrong index");
            return ptr_[index];
        }

        T* data()
        {
            return ptr_;
        }

      private:
        static size_t strlen_(const T* str)
        {
            size_t len = 0;
            while (str[len] != '\0')
            {
                len++;
            }
            return len;
        }
        void clean_()
        {
            delete[] ptr_;
            ptr_ = new T[1]{'\0'};
            size_ = 0;
        }

        T* ptr_ = new T[1]{'\0'};
        size_t size_;
    };
} // namespace bmstu
