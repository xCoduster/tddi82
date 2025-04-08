#include "node.h"

#include <algorithm>
#include <queue>
#include <set>

Node::Node(int value)
    : value { value }
{
}

void Node::insert(Node* node)
{
    auto it = std::find(neighbours.begin(), neighbours.end(), node);

    // det får inte finnas dubletter av länkar
    if (it == neighbours.end())
    {
        neighbours.push_back(node);
    }
}

void Node::remove(Node* node)
{
    // om länken finns: ta bort den, annars: gör ingenting
    auto it = std::remove(neighbours.begin(), neighbours.end(), node);
    neighbours.erase(it, neighbours.end());
}

std::vector<Node*>::const_iterator Node::begin() const
{
    return neighbours.cbegin();
}

std::vector<Node*>::const_iterator Node::end() const
{
    return neighbours.cend();
}

std::vector<Node*> get_all_nodes(Node* root)
{
    // Denna funktion går igenom alla noder och lägger in dem i `nodes` vektorn.
    // Notera: för att detta ska gå att göra så måste den hålla koll på vilka
    // noder som redan har besökts eftersom att flera noder kan länka till samma
    // nod.
    //
    // Om den inte håller koll på detta finns det en risk, i värsta fall, att
    // funktionen hamnar i en evig loop och, i bästa fall, att funktionen
    // producerar dubletter i resultatet.
    //
    // Detta löser funktionen genom att ha en std::set som håller koll på vilka
    // noder som redan har besökts, och en std::vector håller koll på vilka noder
    // som ska besökas härnäst.

    std::vector<Node*> nodes { };

    // Notera att std::set kräver att datatypen går att jämföra med operator<
    // (vilket counted_ptr inte har), så här måste vi använda vanliga pekare fortfarande.

    std::set<Node*> visited { };
    std::queue<Node*> to_visit { };

    to_visit.push(root);
    while (!to_visit.empty())
    {
        Node* node { to_visit.front() };
        to_visit.pop(); // nu blir noden besökt, så den tas bort

        if (visited.count(node) == 0)
        {
            visited.insert(node);
            nodes.push_back(node);

            // vi lägger till alla grannar till `to_visit`
            for (Node* next : *node)
                to_visit.push(next);
        }
    }

    return nodes;
}
