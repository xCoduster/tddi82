#ifndef NODE_H_
#define NODE_H_

#include <vector>

class Node
{
public:
    
    Node(int value = 0);

    void insert(Node* node);
    void remove(Node* node);

    std::vector<Node*>::const_iterator begin() const;
    std::vector<Node*>::const_iterator end() const;

public:

    int value { };

private:

    std::vector<Node*> neighbours { };

};

std::vector<Node*> get_all_nodes(Node* root);

#endif
