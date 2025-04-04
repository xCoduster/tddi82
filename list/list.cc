#include "list.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

template <typename T>
List<T>::List() : sentinel{nullptr}
{
    sentinel = self_referencing();
}

template <typename T>
List<T>::List(std::initializer_list<T> elems) : sentinel{nullptr}
{
    sentinel = self_referencing();

    for (T v : elems)
    {
        insert(v);
    }
}

// special: copy constructor
template <typename T>
List<T>::List(List const& other) : sentinel{nullptr}
{
    sentinel = self_referencing();

    Node* curr{other.sentinel->next};
    while (curr != other.sentinel)
    {
        push_back(curr->elem);
        curr = curr->next;
    }
}

// special: copy assignment
template <typename T>
List<T>& List<T>::operator=(List const& rhs)
{
    while (!is_empty())
    {
        pop_back();
    }

    Node* curr{rhs.sentinel->next};
    while (curr != rhs.sentinel)
    {
        push_back(curr->elem);
        curr = curr->next;
    }

    return *this;
}

// special: destructor
template <typename T>
List<T>::~List()
{
    while (!is_empty())
    {
        pop_back();
    }

    delete sentinel;
}

// special: move constructor
template <typename T>
List<T>::List(List&& other) : sentinel{nullptr}
{
    Node* empty_list_sentinel = self_referencing();
    sentinel = other.sentinel;
    other.sentinel = empty_list_sentinel;
}

// special: move assignment
template <typename T>
List<T>& List<T>::operator=(List&& rhs)
{
    while (!is_empty())
    {
        pop_back();
    }

    delete sentinel;

    Node* empty_list_sentinel = self_referencing();
    sentinel = rhs.sentinel;
    rhs.sentinel = empty_list_sentinel;
    return *this;
}

template <typename T>
void List<T>::insert(T elem)
{
    Node* new_node = new Node{elem, nullptr, nullptr};

    Node* curr{sentinel->next};

    while (curr != sentinel)
    {
        if (elem <= curr->elem)
        {
            add_node(curr->prev, new_node);
            return;
        }

        curr = curr->next;
    }

    // Slutet på listan
    add_node(sentinel->prev, new_node);
}

template <typename T>
void List<T>::remove(int index)
{
    Node* curr{sentinel->next};
    for (int i{0}; i < index; i++)
    {
        if (curr == sentinel)
        {
            throw std::runtime_error("Index finns inte i listan!");
        }

        curr = curr->next;
    }

    remove_node(curr);
    delete curr;
}

template <typename T>
bool List<T>::is_empty() const
{
    return sentinel == sentinel->next;
}

template <typename T>
std::size_t List<T>::length() const
{
    std::size_t len{0};

    Node* curr{sentinel->next};
    while (curr != sentinel)
    {
        curr = curr->next;
        len++;
    }

    return len;
}

template <typename T>
T& List<T>::front() const
{
    if (sentinel == sentinel->next)
    {
        throw std::runtime_error("Listan är tom!");
    }

    return sentinel->next->elem;
}

template <typename T>
T& List<T>::back() const
{
    if (sentinel == sentinel->prev)
    {
        throw std::runtime_error("Listan är tom!");
    }

    return sentinel->prev->elem;
}

template <typename T>
T& List<T>::at(int index) const
{
    Node* curr{sentinel->next};
    for (int i{0}; i < index; i++)
    {
        if (curr == sentinel)
        {
            throw std::runtime_error("Index finns inte i listan!");
        }

        curr = curr->next;
    }

    return curr->elem;
}

template <typename T>
std::string List<T>::to_string() const
{
    std::stringstream ss{};

    ss << "[";

    Node* curr{sentinel->next};
    while (curr != sentinel)
    {
        ss << curr->elem;

        if (curr->next != sentinel)
        {
            ss << ", ";
        }

        curr = curr->next;
    }

    ss << "]";
    return ss.str();
}

template <typename T>
List<T> List<T>::sub(std::initializer_list<T> indices) const
{
    List sub{};
    int curr_idx{};
    Node* curr{sentinel->next};
    int prev_index{-1};

    for (const int& index : indices)
    {
        int diff{index - curr_idx};

        if (index < prev_index)
        {
            throw std::logic_error("Index ej sorterade!");
        }
        prev_index = index;

        for (int i{}; i < diff + 1; i++)
        {
            if (curr == sentinel)
            {
                throw std::runtime_error("Givna index fanns ej i listan!");
            }

            curr = curr->next;
            curr_idx++;
        }

        sub.push_back(curr->prev->elem);
    }

    return sub;
}

template <typename T>
void List<T>::push_back(T elem)
{
    auto node = new Node{elem, nullptr, nullptr};
    add_node(sentinel->prev, node);
}

template <typename T>
T List<T>::pop_back()
{
    auto node = sentinel->prev;
    remove_node(node);
    auto elem = node->elem;
    delete node;
    return elem;
}

template <typename T>
void List<T>::add_node(Node* curr_node, Node* new_node)
{
    new_node->next = curr_node->next;
    new_node->prev = curr_node;
    curr_node->next->prev = new_node;
    curr_node->next = new_node;
}

template <typename T>
void List<T>::remove_node(Node* node)
{
    if (node == sentinel)
    {
        throw std::runtime_error("Kan inte ta bort sentinel!");
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = nullptr;
    node->next = nullptr;
}

template <typename T>
typename List<T>::Node* List<T>::self_referencing()
{
    Node* sentinel = new Node{T{}, nullptr, nullptr};
    sentinel->prev = sentinel;
    sentinel->next = sentinel;
    return sentinel;
}

template <typename T>
List<T>::Iterator::Iterator(Node* elem) : elem{elem}
{
}

template <typename T>
typename List<T>::Iterator::reference List<T>::Iterator::operator*() const
{
    return elem->elem;
}

template <typename T>
typename List<T>::Iterator::pointer List<T>::Iterator::operator->() const
{
    return &(elem->elem);
}

template <typename T>
typename List<T>::Iterator& List<T>::Iterator::operator++()
{
    elem = elem->next;
    return *this;
}

template <typename T>
typename List<T>::Iterator List<T>::Iterator::operator++(int)
{
    List<T>::Iterator tmp{*this};
    elem = elem->next;
    return tmp;
}

template <typename T>
typename List<T>::Iterator& List<T>::Iterator::operator--()
{
    elem = elem->prev;
    return *this;
}

template <typename T>
typename List<T>::Iterator List<T>::Iterator::operator--(int)
{
    List<T>::Iterator tmp{*this};
    elem = elem->prev;
    return tmp;
}

template <typename T>
bool List<T>::Iterator::operator==(const Iterator& other) const
{
    return elem == other.elem;
}

template <typename T>
bool List<T>::Iterator::operator!=(const Iterator& other) const
{
    return elem != other.elem;
}

template <typename T>
typename List<T>::Iterator List<T>::begin()
{
    return List<T>::Iterator{sentinel->next};
}

template <typename T>
typename List<T>::Iterator List<T>::end()
{
    return List<T>::Iterator{sentinel};
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& list)
{
    os << list.to_string();

    return os;
}
