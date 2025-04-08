#ifndef NODE_H_
#define NODE_H_

#include "counted_ptr.h"
#include <vector>

template <typename T>
class Node
{
public:
    Node(T value);

    void insert(counted_ptr<Node<T>>& node);
    void remove(counted_ptr<Node<T>>& node);

    typename std::vector<counted_ptr<Node<T>>>::const_iterator begin() const;
    typename std::vector<counted_ptr<Node<T>>>::const_iterator end() const;

public:
    T value;

private:
    std::vector<counted_ptr<Node<T>>> neighbours{};
};

template <typename T>
std::vector<counted_ptr<Node<T>>> get_all_nodes(counted_ptr<Node<T>>& root);

#include "node.tcc"

#endif
