#ifndef COUNTED_PTR_H_
#define COUNTED_PTR_H_

template <typename T>
class counted_ptr
{
public:
    counted_ptr();
    counted_ptr(T* ptr);

    ~counted_ptr();

    counted_ptr(const counted_ptr& other);
    counted_ptr& operator=(const counted_ptr& rhs);

    counted_ptr(counted_ptr&& other);
    counted_ptr& operator=(counted_ptr&& rhs);

    T& operator*();
    const T& operator*() const;

    T* operator->();
    const T* operator->() const;

    bool operator==(const T& rhs) const;
    bool operator!=(const T& rhs);

    int use_count() const;

    T* get();
    const T* get() const;

private:
    void decr();
    void incr();

    T* ptr;
    int* counter;
};

#include "counted_ptr.tcc"

#endif