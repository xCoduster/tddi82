#include "list.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

// Komplettering (bonus): Det är inte tillåtet att använda komponenter från standardbiblioteket
//   (utöver std::initalizer_list) för att lösa uppgiften.

List::List() : sentinel{nullptr}
{
    sentinel = self_referencing();
}

List::List(std::initializer_list<int> elems) : sentinel{nullptr}
{
    sentinel = self_referencing();

    for (auto v : elems)
    {
        insert(v);
    }
}

// special: copy constructor
List::List(List const& other) : sentinel{nullptr}
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
List& List::operator=(List const& rhs)
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
List::~List()
{
    while (!is_empty())
    {
        pop_back();
    }

    delete sentinel;
}

// special: move constructor
List::List(List&& other) : sentinel{nullptr}
{
    Node* empty_list_sentinel = self_referencing();
    sentinel = other.sentinel;
    other.sentinel = empty_list_sentinel;
}

// special: move assignment
List& List::operator=(List&& rhs)
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

void List::insert(int elem)
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

void List::remove(int index)
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

bool List::is_empty() const
{
    return sentinel == sentinel->next;
}

int List::length() const
{
    int len{0};

    Node* curr{sentinel->next};
    while (curr != sentinel)
    {
        curr = curr->next;
        len++;
    }

    return len;
}

int List::front() const
{
    if (sentinel == sentinel->next)
    {
        throw std::runtime_error("Listan är tom!");
    }

    return sentinel->next->elem;
}

int List::back() const
{
    if (sentinel == sentinel->prev)
    {
        throw std::runtime_error("Listan är tom!");
    }

    return sentinel->prev->elem;
}

int List::at(int index) const
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

std::string List::to_string() const
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

List List::sub(std::initializer_list<int> indices) const
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

void List::push_back(int elem)
{
    auto node = new Node{elem, nullptr, nullptr};
    add_node(sentinel->prev, node);
}

int List::pop_back()
{
    auto node = sentinel->prev;
    remove_node(node);
    auto elem = node->elem;
    delete node;
    return elem;
}

void List::add_node(Node* curr_node, Node* new_node)
{
    new_node->next = curr_node->next;
    new_node->prev = curr_node;
    curr_node->next->prev = new_node;
    curr_node->next = new_node;
}

void List::remove_node(Node* node)
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

List::Node* List::self_referencing()
{
    Node* sentinel = new Node{0, nullptr, nullptr};
    sentinel->prev = sentinel;
    sentinel->next = sentinel;
    return sentinel;
}

List::Iterator::Iterator(Node* elem) : elem{elem}
{
}

List::Iterator::reference List::Iterator::operator*() const
{
    return elem->elem;
}

List::Iterator::pointer List::Iterator::operator->() const
{
    return &(elem->elem);
}

List::Iterator& List::Iterator::operator++()
{
    elem = elem->next;
    return *this;
}

List::Iterator List::Iterator::operator++(int)
{
    List::Iterator tmp{*this};
    elem = elem->next;
    return tmp;
}

List::Iterator& List::Iterator::operator--()
{
    elem = elem->prev;
    return *this;
}

List::Iterator List::Iterator::operator--(int)
{
    List::Iterator tmp{*this};
    elem = elem->prev;
    return tmp;
}

bool List::Iterator::operator==(const Iterator& other) const
{
    return elem == other.elem;
}

bool List::Iterator::operator!=(const Iterator& other) const
{
    return elem != other.elem;
}

List::Iterator List::begin()
{
    return List::Iterator{sentinel->next};
}

List::Iterator List::end()
{
    return List::Iterator{sentinel};
}

std::ostream& operator<<(std::ostream& os, const List& list)
{
    os << list.to_string();

    return os;
}
