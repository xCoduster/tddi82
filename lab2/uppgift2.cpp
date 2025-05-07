#include <iostream>

#include "counted_ptr.h"
#include "node.h"

//Komplettering: Ändra så att uppgift2 använder sig av std::string enligt uppgiftsbeskrivningen.

counted_ptr<Node<int>> create_example()
{
    counted_ptr<Node<int>> n0{new Node{0}};
    counted_ptr<Node<int>> n1{new Node{1}};
    counted_ptr<Node<int>> n2{new Node{2}};
    counted_ptr<Node<int>> n3{new Node{3}};
    counted_ptr<Node<int>> n4{new Node{4}};
    counted_ptr<Node<int>> n5{new Node{5}};

    n0->insert(n1);
    n0->insert(n2);
    n1->insert(n5);
    n2->insert(n3);
    n2->insert(n4);
    n3->insert(n4);
    n3->insert(n5);

    return n0;
}

void print_nodes(std::vector<counted_ptr<Node<int>>> nodes)
{
    for (counted_ptr<Node<int>> node : nodes)
    {
        std::cout << node->value << " ";
    }
    std::cout << std::endl;
}

int main()
{
    counted_ptr<Node<int>> root{create_example()};
    std::vector<counted_ptr<Node<int>>> nodes{get_all_nodes(root)};

    std::cout << "After insertions: " << std::endl;
    print_nodes(nodes);

    root->remove(nodes[2]);

    nodes = get_all_nodes(root);

    std::cout << "After removal: " << std::endl;
    print_nodes(nodes);
}
