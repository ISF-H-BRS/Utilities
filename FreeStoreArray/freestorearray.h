// ============================================================================================== //
//                                                                                                //
//   This file is part of the ISF Utilities collection.                                           //
//                                                                                                //
//   Author:                                                                                      //
//   Marcel Hasler <mahasler@gmail.com>                                                           //
//                                                                                                //
//   Copyright (c) 2022 - 2023                                                                    //
//   Bonn-Rhein-Sieg University of Applied Sciences                                               //
//                                                                                                //
//   Redistribution and use in source and binary forms, with or without modification,             //
//   are permitted provided that the following conditions are met:                                //
//                                                                                                //
//   1. Redistributions of source code must retain the above copyright notice,                    //
//      this list of conditions and the following disclaimer.                                     //
//                                                                                                //
//   2. Redistributions in binary form must reproduce the above copyright notice,                 //
//      this list of conditions and the following disclaimer in the documentation                 //
//      and/or other materials provided with the distribution.                                    //
//                                                                                                //
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"                  //
//   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED            //
//   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.           //
//   IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,             //
//   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT           //
//   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR           //
//   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,            //
//   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)           //
//   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE                   //
//   POSSIBILITY OF SUCH DAMAGE.                                                                  //
//                                                                                                //
// ============================================================================================== //

#pragma once

#include <initializer_list>
#include <memory>
#include <stdexcept>

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A = std::allocator<T>>
class FreeStoreArray
{
public:
    using value_type             = T;
    using allocator_type         = A;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using iterator               = value_type*;
    using const_iterator         = const value_type*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    FreeStoreArray();
    FreeStoreArray(const FreeStoreArray& other);
    FreeStoreArray(FreeStoreArray&& other) noexcept;
    FreeStoreArray(std::initializer_list<T> list);
    ~FreeStoreArray() noexcept;

    auto operator=(const FreeStoreArray& other) -> FreeStoreArray&;
    auto operator=(FreeStoreArray&& other) noexcept -> FreeStoreArray&;

    void fill(const T& value);

    auto front() -> reference;
    auto front() const -> const_reference;

    auto back() -> reference;
    auto back() const -> const_reference;

    auto at(size_t n) -> reference;
    auto at(size_t n) const -> const_reference;

    auto operator[](size_t n) noexcept -> reference;
    auto operator[](size_t n) const noexcept -> const_reference;

    auto data() noexcept -> pointer;
    auto data() const noexcept -> const_pointer;

    auto size() const noexcept -> size_type;
    auto empty() const noexcept -> bool;

    auto begin() noexcept -> iterator;
    auto begin() const noexcept -> const_iterator;

    auto end() noexcept -> iterator;
    auto end() const noexcept -> const_iterator;

    auto rbegin() noexcept -> reverse_iterator;
    auto rbegin() const noexcept -> const_reverse_iterator;

    auto rend() noexcept -> reverse_iterator;
    auto rend() const noexcept -> const_reverse_iterator;

    auto cbegin() const noexcept -> const_iterator;
    auto cend() const noexcept -> const_iterator;

    auto crbegin() const noexcept -> const_reverse_iterator;
    auto crend() const noexcept -> const_reverse_iterator;

private:
    void checkRange(size_t n) const;
    void destruct();

private:
    T* m_data = nullptr;
};

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
FreeStoreArray<T,N,A>::FreeStoreArray()
    : m_data(A().allocate(N))
{
    std::uninitialized_fill_n(m_data, N, T{});
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
FreeStoreArray<T,N,A>::FreeStoreArray(const FreeStoreArray& other)
    : m_data(A().allocate(N))
{
    std::uninitialized_copy(other.begin(), other.end(), m_data);
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
FreeStoreArray<T,N,A>::FreeStoreArray(FreeStoreArray&& other) noexcept
    : m_data(other.m_data)
{
    other.m_data = nullptr;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
FreeStoreArray<T,N,A>::FreeStoreArray(std::initializer_list<T> list)
    : m_data(A().allocate(N))
{
    std::uninitialized_copy(list.begin(), list.end(), m_data);
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
FreeStoreArray<T,N,A>::~FreeStoreArray() noexcept
{
    destruct();
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::operator=(const FreeStoreArray& other) -> FreeStoreArray&
{
    if (m_data)
        std::copy(other.begin(), other.end(), m_data);
    else
    {
        m_data = A().allocate(N);
        std::uninitialized_copy(other.begin(), other.end(), m_data);
    }

    return *this;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::operator=(FreeStoreArray&& other) noexcept -> FreeStoreArray&
{
    destruct();

    m_data = other.m_data;
    other.m_data = nullptr;

    return *this;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
void FreeStoreArray<T,N,A>::fill(const T& value)
{
    std::fill(begin(), end(), value);
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::front() -> reference
{
    return at(0);
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::front() const -> const_reference
{
    return at(0);
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::back() -> reference
{
    return at(N - 1);
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::back() const -> const_reference
{
    return at(N - 1);
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::at(size_t n) -> reference
{
    checkRange(n);
    return m_data[n];
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::at(size_t n) const -> const_reference
{
    checkRange(n);
    return m_data[n];
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::operator[](size_t n) noexcept -> reference
{
    return m_data[n];
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::operator[](size_t n) const noexcept -> const_reference
{
    return m_data[n];
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::data() noexcept -> pointer
{
    return m_data;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::data() const noexcept -> const_pointer
{
    return m_data;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::size() const noexcept -> size_type
{
    return m_data ? N : 0;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::empty() const noexcept -> bool
{
    return size() == 0;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::begin() noexcept -> pointer
{
    return m_data;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::begin() const noexcept -> const_pointer
{
    return m_data;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::end() noexcept -> pointer
{
    return m_data + size();
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::end() const noexcept -> const_pointer
{
    return m_data + size();
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::rbegin() noexcept -> reverse_iterator
{
    return m_data + size() - 1;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::rbegin() const noexcept -> const_reverse_iterator
{
    return m_data + size() - 1;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::rend() noexcept -> reverse_iterator
{
    return m_data - 1;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::rend() const noexcept -> const_reverse_iterator
{
    return m_data - 1;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::cbegin() const noexcept -> const_iterator
{
    return m_data;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::cend() const noexcept -> const_iterator
{
    return m_data + size();
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::crbegin() const noexcept -> const_reverse_iterator
{
    return m_data + size() - 1;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
auto FreeStoreArray<T,N,A>::crend() const noexcept -> const_reverse_iterator
{
    return m_data - 1;
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
void FreeStoreArray<T,N,A>::checkRange(size_t n) const
{
    if (!m_data)
        throw std::runtime_error("FreeStoreArray: Invalid access to moved-from container.");

    if (n >= N)
        throw std::out_of_range("FreeStoreArray: Subscript out of range.");
}

// ---------------------------------------------------------------------------------------------- //

template <typename T, size_t N, class A>
void FreeStoreArray<T,N,A>::destruct()
{
    if (m_data)
    {
        if constexpr (!std::is_trivial<T>::value)
        {
            for (size_t i = 0; i < N; ++i)
                m_data[i].~T();
        }

        A().deallocate(m_data, N);
    }
}

// ---------------------------------------------------------------------------------------------- //
