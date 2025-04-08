#include "node.h"

#include <iostream>

#include "counted_ptr.h"

Node* create_example()
{
    Node* n0 { new Node { 0 } };
    Node* n1 { new Node { 1 } };
    Node* n2 { new Node { 2 } };
    Node* n3 { new Node { 3 } };
    Node* n4 { new Node { 4 } };
    Node* n5 { new Node { 5 } };

    n0->insert(n1);
    n0->insert(n2);
    n1->insert(n5);
    n2->insert(n3);
    n2->insert(n4);
    n3->insert(n4);
    n3->insert(n5);

    return n0;
}

void print_nodes(std::vector<Node*> nodes)
{
    for (Node* node : nodes)
    {
        std::cout << node->value << " ";
    }
    std::cout << std::endl;
}

int main()
{
    Node* root { create_example() };
    std::vector<Node*> nodes { get_all_nodes(root) };

    std::cout << "After insertions: " << std::endl;
    print_nodes(nodes);

    root->remove(nodes[2]);

    nodes = get_all_nodes(root);

    std::cout << "After removal: " << std::endl;
    print_nodes(nodes);
}
