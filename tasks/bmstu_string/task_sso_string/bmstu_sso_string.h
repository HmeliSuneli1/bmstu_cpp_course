#pragma once

#include <exception>
#include <iostream>

namespace bmstu
{
    template <typename T> class basic_string;

    using string = basic_string<char>;
    using wstring = basic_string<wchar_t>;
    using u16string = basic_string<char16_t>;
    using u32string = basic_string<char32_t>;

    template <typename T> class basic_string
    {
      private:
        static constexpr size_t SSO_CAPACITY =
            (sizeof(T*) + sizeof(size_t) + sizeof(size_t)) / sizeof(T) - 1;

        struct LongString
        {
            T* ptr;
            size_t size;
            size_t capacity;
        };

        struct ShortString
        {
            T buffer[SSO_CAPACITY + 1];
            unsigned char size;
        };

        union Data
        {
            LongString long_str;
            ShortString short_str;

            Data() {}
            ~Data() {}
        };

        Data data_;
        bool is_long_;

        bool is_long() const
        {
            return is_long_;
        }

        T* get_ptr()
        {
            return is_long_ ? data_.long_str.ptr : data_.short_str.buffer;
        }

        const T* get_ptr() const
        {
            return is_long_ ? data_.long_str.ptr : data_.short_str.buffer;
        }

        size_t get_size() const
        {
            return is_long_ ? data_.long_str.size : data_.short_str.size;
        }

        size_t get_capacity() const
        {
            return is_long_ ? data_.long_str.capacity : SSO_CAPACITY;
        }

      public:
        basic_string() {
            is_long_ = false;
            data_.short_str.size = 0;
            data_.short_str.buffer[0] = '\0';
        }

        basic_string(size_t size)
        {
            if (size <= SSO_CAPACITY)
            {
                is_long_ = false;
                data_.short_str.size = static_cast<unsigned char>(size);
                for (size_t i = 0; i < size; i++)
                {
                    data_.short_str.buffer[i] = ' ';
                }
                data_.short_str.buffer[size] = '\0';
            }
            else
            {
                is_long_ = true;
                data_.long_str.size = size;
                data_.long_str.capacity = size;
                data_.long_str.ptr = new T[size + 1];
                for (size_t i = 0; i < size; i++)
                {
                    data_.long_str.ptr[i] = ' ';
                }
                data_.long_str.ptr[size] = '\0';
            }
        }

        basic_string(std::initializer_list<T> il)
        {
            size_t size = il.size();
            if (size <= SSO_CAPACITY)
            {
                is_long_ = false;
                data_.short_str.size = static_cast<unsigned char>(size);
                for (size_t i = 0; i < size; i++)
                {
                    data_.short_str.buffer[i] = *(il.begin() + i);
                }
                data_.short_str.buffer[size] = '\0';
            }
            else
            {
                is_long_ = true;
                data_.long_str.size = size;
                data_.long_str.capacity = size;
                data_.long_str.ptr = new T[size + 1];
                for (size_t i = 0; i < size; i++)
                {
                    data_.long_str.ptr[i] = *(il.begin() + i);
                }
                data_.long_str.ptr[size] = '\0';
            }
        }

        basic_string(const T* c_str)
        {
            size_t size = strlen_(c_str);
            if (size <= SSO_CAPACITY)
            {
                is_long_ = false;
                data_.short_str.size = static_cast<unsigned char>(size);
                for (size_t i = 0; i < size; i++)
                {
                    data_.short_str.buffer[i] = c_str[i];
                }
                data_.short_str.buffer[size] = '\0';
            }
            else
            {
                is_long_ = true;
                data_.long_str.size = size;
                data_.long_str.capacity = size;
                data_.long_str.ptr = new T[size + 1];
                for (size_t i = 0; i < size; i++)
                {
                    data_.long_str.ptr[i] = c_str[i];
                }
                data_.long_str.ptr[size] = '\0';
            }
        }

        basic_string(const basic_string& other)
        {
            if (other.is_long())
            {
                is_long_ = true;
                data_.long_str.size = other.data_.long_str.size;
                data_.long_str.capacity = other.data_.long_str.capacity;
                data_.long_str.ptr = new T[data_.long_str.capacity + 1];
                for (size_t i = 0; i < data_.long_str.size; i++)
                {
                    data_.long_str.ptr[i] = other.data_.long_str.ptr[i];
                }
                data_.long_str.ptr[data_.long_str.size] = '\0';
            }
            else
            {
                is_long_ = false;
                data_.short_str.size = other.data_.short_str.size;
                for (size_t i = 0; i < data_.short_str.size; i++)
                {
                    data_.short_str.buffer[i] = other.data_.short_str.buffer[i];
                }
                data_.short_str.buffer[data_.short_str.size] = '\0';
            }
        }

            basic_string(basic_string&& dying) noexcept {
                if (!dying.is_long())
                {
                    is_long_ = false;
                    data_.short_str.size = dying.data_.short_str.size;
            for (size_t i = 0; i < data_.short_str.size; i++)
			{
                        data_.short_str.buffer[i] = dying.data_.short_str.buffer[i];
			}
			data_.short_str.buffer[data_.short_str.size] = '\0';
                }
                else
                {
                    is_long_ = true;
                    data_.long_str.ptr = dying.data_.long_str.ptr;
                    data_.long_str.size = dying.data_.long_str.size;
                    data_.long_str.capacity = dying.data_.long_str.capacity;
                    dying.data_.long_str.ptr = new T[1]{'\0'};
                    dying.data_.long_str.size = 0;
                    dying.data_.long_str.capacity = 0;
                }
            }

            ~basic_string()
            {
                if (is_long_)
                {
                    delete[] data_.long_str.ptr;
                }
            }

            const T* c_str() const
            {
                return get_ptr();
            }

            size_t size() const
            {
                return get_size();
            }

            bool is_using_sso() const
            {
                return !is_long_;
            }

            size_t capacity() const
            {
                return get_capacity();
            }

            basic_string& operator=(basic_string&& other) noexcept
            {
                clean_();
                if (other.is_long())
                {
                    data_.long_str.capacity = other.data_.long_str.capacity;
			        data_.long_str.size = other.data_.long_str.capacity;
			        data_.long_str.ptr = new T[data_.long_str.capacity];
                }
                else
                {
			        data_.short_str.size = other.data_.short_str.size;
		        }

		        for (size_t i = 0; i<=size(); i++){
			        get_ptr()[i] = other.get_ptr()[i];
		        }
		
                return *this;
            }

            basic_string& operator=(const T* c_str)
            {
                clean_();
                size_t size = strlen_(c_str);
                if (size <= SSO_CAPACITY)
                {
                    is_long_ = false;
                    data_.short_str.size = static_cast<unsigned char>(size);
                    for (size_t i = 0; i < size; i++)
                    {
                        data_.short_str.buffer[i] = c_str[i];
                    }
                    data_.short_str.buffer[size] = '\0';
                }
                else
                {
                    is_long_ = true;
                    data_.long_str.size = size;
                    data_.long_str.capacity = size;
                    data_.long_str.ptr = new T[size + 1];
                    for (size_t i = 0; i < size; i++)
                    {
                        data_.long_str.ptr[i] = c_str[i];
                    }
                    data_.long_str.ptr[size] = '\0';
                }
                return *this;
            }

            basic_string& operator=(const basic_string& other)
            {
                clean_();
                if (is_long_) {
                    data_.long_str.capacity = other.data_.long_str.capacity;
			        data_.long_str.size = other.data_.long_str.capacity;
			        data_.long_str.ptr = new T[data_.long_str.capacity];
                }
                else {
			        data_.short_str.size = other.data_.short_str.size;
		        }

		        for (size_t i = 0; i<=size(); i++){
			        get_ptr()[i] = other.get_ptr()[i];
		        }
		
                return *this;
            }

            friend basic_string<T> operator+(const basic_string<T>& left,
                                             const basic_string<T>& right)
            {
                if (left.size() + right.size() <= SSO_CAPACITY)
                {
                    basic_string<T> new_string;
                    new_string.is_long_ = false;
                    new_string.data_.short_str.size =
                        static_cast<unsigned char>(left.size() + right.size());
                    for (size_t i = 0; i < left.size(); i++)
                    {
                        new_string.data_.short_str.buffer[i] = left.get_ptr()[i];
                    }
                    for (size_t i = 0; i < right.size(); i++)
                    {
                        new_string.data_.short_str.buffer[i + left.size()] = right.get_ptr()[i];
                    }
                    new_string.data_.short_str.buffer[new_string.data_.short_str.size] = '\0';
                    return new_string;
                }
                else
                {
                    basic_string<T> new_string;
                    new_string.is_long_ = true;
                    new_string.data_.long_str.size = left.size() + right.size();
                    new_string.data_.long_str.capacity = new_string.data_.long_str.size;
                    new_string.data_.long_str.ptr = new T[new_string.data_.long_str.capacity + 1];
                    for (size_t i = 0; i < left.size(); i++)
                    {
                        new_string.data_.long_str.ptr[i] = left.get_ptr()[i];
                    }
                    for (size_t i = 0; i < right.size(); i++)
                    {
                        new_string.data_.long_str.ptr[i + left.size()] = right.get_ptr()[i];
                    }
                    new_string.data_.long_str.ptr[new_string.data_.long_str.size] = '\0';
                    return new_string;
                }
            }
                template <typename S> friend S& operator<<(S& os, const basic_string& obj)
                {
                    os << obj.c_str();
                    return os;
                }

                template <typename S> friend S& operator>>(S& is, basic_string& obj)
                {
                    obj.clean_();
                    obj = basic_string();
                    T character;
                    while (is.get(character))
                    {
                        obj += character;
                    }
                    return is;
                }

                basic_string& operator+=(const basic_string& other)
                {
                    T* new_ptr = new T[size() + other.size() + 1];
                    for (size_t i = 0; i < size(); i++)
                    {
                        new_ptr[i] = get_ptr()[i];
                    }
                    for (size_t i = 0; i < other.size(); i++)
                    {
                        new_ptr[i + size()] = other.get_ptr()[i];
                    }
                    new_ptr[size() + other.size()] = '\0';
                    delete[] get_ptr();
                    if (is_long_)
                    {
                        data_.long_str.ptr = new_ptr;
                        data_.long_str.size += other.size();
                    }
                    else
                    {
                        is_long_ = true;
                        data_.long_str.ptr = new_ptr;
                        data_.long_str.size = size() + other.size();
                        data_.long_str.capacity = size() + other.size();
                    }
                    return *this;
                }

                basic_string& operator+=(T symbol)
                {
                    T* new_ptr = new T[size() + 2];
                    for (size_t i = 0; i < size(); i++)
                    {
                        new_ptr[i] = get_ptr()[i];
                    }
                    new_ptr[size()] = symbol;
                    new_ptr[size() + 1] = '\0';
                    if (is_long_)
                    {
                        data_.long_str.ptr = new_ptr;
                        data_.long_str.size += 1;
                    }
                    else
                    {
                        if (size() + 1 <= SSO_CAPACITY)
                        {
                            data_.short_str.buffer[size()] = symbol;
                            data_.short_str.size += 1;
                            data_.short_str.buffer[data_.short_str.size] = '\0';
                            delete[] new_ptr;
                        }
                        else
                        {
                            is_long_ = true;
                            data_.long_str.ptr = new_ptr;
                            data_.long_str.size = size() + 1;
                            data_.long_str.capacity = size() + 1;
                        }
                    }
                    return *this;
                }

                T& operator[](size_t index) noexcept
                {
                    if (is_long_)
                    {
                        return data_.long_str.ptr[index];
                    }
                    else
                    {
                        return data_.short_str.buffer[index];
                    }
                }

                T& at(size_t index)
                {
                    if (index < 0 || index >= get_size())
                        throw std::out_of_range("Wrong index");
                    return get_ptr()[index];
                }

                T* data()
                {
                    return get_ptr();
                }


              private:
                static size_t strlen_(const T* str) { 
		            size_t len = 0;
		            while (str[len]!=T()){
			            len++;
		            }
		            return len; 
	            }

                void clean_()
                {
                    if (is_long())
                    {
                        delete[] data_.long_str.ptr;
                    }
                }
            };
} // namespace bmstu