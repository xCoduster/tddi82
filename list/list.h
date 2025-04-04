#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>

#include <cstddef>
#include <iterator>

template <typename T>
class List
{
public:
    List();
    List(std::initializer_list<T> elems);

    // special: copy constructor
    List(List const& other);
    // special: copy assignment
    List& operator=(List const& rhs);
    // special: destructor
    ~List();
    // special: move constructor
    List(List&& other);
    // special: move assignment
    List& operator=(List&& rhs);

    void insert(T elem);
    void remove(int index);

    bool is_empty() const;
    std::size_t length() const;
    T& front() const;
    T& back() const;
    T& at(int index) const;

    std::string to_string() const;

    List sub(std::initializer_list<T> indices) const;

private:
    struct Node
    {
        T elem;
        Node* next;
        Node* prev;
    };

    class Iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        Iterator(Node* elem);

        reference operator*() const;
        pointer operator->() const;

        Iterator& operator++();
        Iterator operator++(int);

        Iterator& operator--();
        Iterator operator--(int);

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        List::Node* elem;
    };

    void push_back(T elem);
    T pop_back();
    void add_node(Node* curr_node, Node* new_node);
    void remove_node(Node* node);
    Node* self_referencing();

    Node* sentinel;

public:
    Iterator begin();
    Iterator end();
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& list);

// Templates must be written in the header file
// Renamed to list.cc to not be included in make
#include "list.cc"

#endif
