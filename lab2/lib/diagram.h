#ifndef LIB_DIAGRAM_H_
#define LIB_DIAGRAM_H_

#include "lib/deduce.h"
#include "lib/math.h"

#include <unordered_set>

#include <SFML/Graphics.hpp>

// Settings for the visual elements of the diagram
struct Diagram_Settings
{

    // the radius of each node in the diagram, measured in pixels
    float node_radius { 16.0f };

    // the distance between the base and the tip of each arrow tip, measured in pixels
    float arrow_length { 8.0f };

    // the minimum distance between two nodes
    float node_distance { 5.0f * node_radius };

    // the thickness of the link curves, measured in pixels
    float link_width { 2.0f };

    // the size of the "break" between links that are crossing each other, measured in pixels
    float link_gap_size { 2.0f };

    // the angular gap between ports on nodes
    float port_angular_gap { pi / 8.0f };

    // the color scheme of the diagram
    sf::Color background_color { sf::Color::White };
    sf::Color foreground_color { sf::Color::Black };
};

// Internal representation of the diagram
struct Diagram_Node
{
    // map incoming and outgoing link to id's of the opposite side of the link
    std::unordered_set<std::size_t> outgoing { };
    std::unordered_set<std::size_t> incoming { };

    // the externally assigned id of the node
    std::size_t id { };

    // the index of the node
    std::size_t index { 0 };

    // the assigned position of the node
    sf::Vector2f position { };
};

void draw_diagram(sf::RenderTarget& target, std::vector<Node_Ptr> const& roots,
                  Diagram_Settings const& settings = {});

#endif
