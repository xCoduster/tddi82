#include "counted_ptr.h"

// Komplettering: Ändra så att uppgift2 använder sig av std::string enligt uppgiftsbeskrivningen.

// Komplettering: Istället för att kolla ptr mot nullptr i incr, decr och use_count kolla mot counter pekaren direkt.
// Komplettering: Självtilldelning innebär felaktigt beteende i tilldelningsoperatorerna.

template <typename T>
counted_ptr<T>::counted_ptr()
    : ptr{nullptr}, counter{nullptr}
{
}

template <typename T>
counted_ptr<T>::counted_ptr(T* ptr)
    : ptr{ptr}, counter{new int{1}}
{
}

template <typename T>
counted_ptr<T>::~counted_ptr()
{
    decr();
}

template <typename T>
counted_ptr<T>::counted_ptr(const counted_ptr<T>& other)
    : ptr{other.ptr}, counter{other.counter}
{
    incr();
}

template <typename T>
counted_ptr<T>& counted_ptr<T>::operator=(const counted_ptr<T>& rhs)
{
    if (&rhs != this)
    {
        decr();
        ptr = rhs.ptr;
        counter = rhs.counter;
        incr();
    }
    return *this;
}

template <typename T>
counted_ptr<T>::counted_ptr(counted_ptr<T>&& rhs)
    : ptr{rhs.ptr}, counter{rhs.counter}
{
    rhs.ptr = nullptr;
    rhs.counter = nullptr;
}

template <typename T>
counted_ptr<T>& counted_ptr<T>::operator=(counted_ptr<T>&& rhs)
{
    if (&rhs != this)
    {
        decr();
        ptr = rhs.ptr;
        counter = rhs.counter;
        rhs.ptr = nullptr;
        rhs.counter = nullptr;
    }
    return *this;
}

template <typename T>
counted_ptr<T>& counted_ptr<T>::operator=(std::nullptr_t rhs)
{
    decr();
    ptr = nullptr;
    counter = nullptr;

    return *this;
}

template <typename T>
T& counted_ptr<T>::operator*()
{
    return *ptr;
}

template <typename T>
const T& counted_ptr<T>::operator*() const
{
    return *ptr;
}

template <typename T>
T* counted_ptr<T>::operator->()
{
    return ptr;
}

template <typename T>
const T* counted_ptr<T>::operator->() const
{
    return ptr;
}

template <typename T>
bool counted_ptr<T>::operator==(const counted_ptr<T>& rhs) const
{
    return ptr == rhs.ptr;
}

template <typename T>
bool counted_ptr<T>::operator!=(const counted_ptr<T>& rhs) const
{
    return ptr != rhs.ptr;
}

template <typename T>
bool counted_ptr<T>::operator==(const T* rhs) const
{
    return ptr == rhs;
}

template <typename T>
bool counted_ptr<T>::operator!=(const T* rhs) const
{
    return ptr != rhs;
}

template <typename T>
int counted_ptr<T>::use_count() const
{
    if (counter == nullptr)
    {
        return 0;
    }

    return *counter;
}

template <typename T>
T* counted_ptr<T>::get()
{
    return ptr;
}

template <typename T>
const T* counted_ptr<T>::get() const
{
    return ptr;
}

template <typename T>
void counted_ptr<T>::incr()
{
    if (counter != nullptr)
    {
        ++(*counter);
    }
}

template <typename T>
void counted_ptr<T>::decr()
{
    if (counter != nullptr)
    {
        if (--(*counter) == 0)
        {
            delete counter;
            delete ptr;
        }
    }
}