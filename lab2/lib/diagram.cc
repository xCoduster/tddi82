#include "lib/diagram.h"
#include "lib/math.h"
#include "lib/gfx.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>
#include <memory>

namespace
{

    bool compare_nodes(Node_Ptr const& n1, Node_Ptr const& n2)
    {
        int v1 { -1 };
        int v2 { -1 };

        if (n1 != nullptr) v1 = n1->value;
        if (n2 != nullptr) v2 = n2->value;

        return v1 < v2;
    }

    // extracts all nodes that are linked *from* all the nodes in `nodes`
    std::vector<Node_Ptr> extract_all_nodes(std::vector<Node_Ptr> nodes)
    {
        std::vector<Node_Ptr> result { };
        std::sort(nodes.begin(), nodes.end(), compare_nodes);

        while (!nodes.empty())
        {
            // find the first cluster of nodes from the `nodes` list
            auto cluster = get_all_nodes(nodes.front());
            std::sort(cluster.begin(), cluster.end(), compare_nodes);

            // add all newly found nodes in this cluster to `result`
            std::set_difference(cluster.begin(), cluster.end(),
                                result .begin(), result .end(),
                                std::back_inserter(result), compare_nodes);

            // ensure that the result is sorted after this
            std::sort(result.begin(), result.end(), compare_nodes);

            // remove all nodes found in this cluster from the `nodes` list
            auto it = std::set_difference(nodes  .begin(), nodes  .end(),
                                          cluster.begin(), cluster.end(),
                                          nodes  .begin(), compare_nodes);
            nodes.erase(it, nodes.end());
        }

        return result;
    }

    // translate the directed graph induced by the `roots` vector (i.e. all
    // nodes reachable from the nodes in `roots`) to the internal representation
    // of the diagram drawing module.
    std::vector<Diagram_Node> construct_diagram(std::vector<Node_Ptr> const& roots)
    {
        // get all nodes reachable from the nodes in `roots`
        std::vector<Node_Ptr> nodes { extract_all_nodes(roots) };

        // construct the diagram representation
        std::vector<Diagram_Node> result(nodes.size());

        std::unordered_map<Node_Type const*, std::size_t> index_map { };

        for (std::size_t i { 0 }; i < nodes.size(); ++i)
        {
            Node_Ptr node { nodes[i] };
            result[i].id = node->value;
            result[i].index = i;

            index_map[std::addressof(*node)] = i;
        }

        for (Node_Ptr node : nodes)
        {
            std::size_t i { index_map[std::addressof(*node)] };
            for (Node_Ptr neighbour : (*node))
            {
                std::size_t j { index_map[std::addressof(*neighbour)] };
                result[i].outgoing.insert(j);
                result[j].incoming.insert(i);
            }
        }

        return result;
    }

    // load the font
    static sf::Font font {
        [](std::string const& file)
        {
            sf::Font font { };
            if (!font.loadFromFile(file))
                std::exit(1);
            return font;
        }("lib/font.otf")
        };

    class Diagram
    {
    public:

        Diagram(std::vector<Node_Ptr> const& roots,
                Diagram_Settings      const& settings)
            : nodes    { construct_diagram(roots) },
              settings { settings }
        {
        }

        void draw(sf::RenderTarget& target)
        {
            assign_positions(target);

            // keep track of all drawn links
            std::set<std::pair<std::size_t, std::size_t>> links { };
            
            // Draw all links
            for (Diagram_Node const& from : nodes)
            {
                for (std::size_t i : from.outgoing)
                {
                    std::pair<std::size_t, std::size_t>  forward(from.id, i);
                    std::pair<std::size_t, std::size_t> backward(i, from.id);                    

                    // only draw this link if it hasn't been drawn before
                    if (links.count(forward) == 0)
                    {
                        Diagram_Node const& to { nodes[i] };
                        draw_link(target, from, to);

                        // insert both directions to block the other direction
                        // when it potentially comes around
                        links.insert( forward);
                        links.insert(backward);
                    }
                }
            }
                
            // Draw all nodes
            for (Diagram_Node const& node : nodes)
                draw_node(target, node);
        }

    private:

        void assign_positions(sf::RenderTarget& target)
        {
            float const count(nodes.size());

            float const width { (count - 1) * settings.node_distance + 2.0f*settings.node_radius };
            sf::Vector2f position { 0.5f*(target.getSize().x - width), 0.5f*target.getSize().y };
            for (Diagram_Node& node : nodes)
            {
                node.position = position;
                position.x += settings.node_distance;
            }
        }

        void draw_node(sf::RenderTarget& target, Diagram_Node const& node) const
        {
            sf::Text text(std::to_string(node.id), font, 1.5f*settings.node_radius);
            text.setFillColor(settings.foreground_color);

            sf::FloatRect bounds { text.getLocalBounds() };
            text.setOrigin(bounds.left + 0.5f*bounds.width,
                           bounds.top  + 0.5f*bounds.height);
            text.setPosition(node.position);

            sf::CircleShape circle { settings.node_radius };
            circle.setFillColor(settings.background_color);

            circle.setOutlineThickness(settings.link_width);
            circle.setOutlineColor(settings.foreground_color);

            circle.setOrigin(circle.getRadius(), circle.getRadius());
            circle.setPosition(node.position);

            target.draw(circle);
            target.draw(text);
        }

    int calculate_link_rank(Diagram_Node const& from, Diagram_Node const& to)
    {
        return std::abs(static_cast<int>(to.index) - static_cast<int>(from.index)) - 1;
    }


        void draw_link(sf::RenderTarget& target, Diagram_Node const& from, Diagram_Node const& to)
        {
            // This function calculates two so called `ports` which are points along
            // the outer edge of the nodes (circles) where the path starts and ends
            // respectively. The idea is that each edge should have its own port at
            // the start- and end node. This is done to reduce overlap between links.
            //
            // The second goal of this function is to bend links in such a way that
            // they are easy to follow visually, i.e. by ensuring that don't overlap
            // more than in a discrete set of intersection points.
            //
            // The main idea for implementing these two goals is to base the port
            // and bending on the `rank` of the `from` and `to` nodes. To do these
            // we combine the ranks of the two nodes into something called a link
            // rank which basically measures how many nodes there are between `from`
            // and `to` and then assign ports further away from the direct path
            // between the nodes based on the size of the link rank, and increase
            // 'bendiness' of links for higher link ranks.

            bool const is_double(from.incoming.count(to.index));
            
            sf::Vector2f const d { normalize(to.position - from.position) };

            int const link_rank { calculate_link_rank(from, to) };

            float const port  { signbit(link_rank % 2) * link_rank };
            float const angle { (port - 0.5f*(link_rank != 0)) * settings.port_angular_gap };

            // the angle between the two nodes.
            float const base_angle { direction(d) };

            // `out` is the position of the port for the link relative to the `from`
            // node, and `in` is equivalent but for the `to` node.
            sf::Vector2f const out { unit(base_angle +       angle ) };
            sf::Vector2f const in  { unit(base_angle + (pi - angle)) };

            // how far into the path must the base of the arrow be
            float const arrow_offset { settings.arrow_length + 2.0f*settings.link_width };

            // calculate control points for cubic bezier curves

            // the endpoints of the bezier curve represent the base of the arrow(s)
            sf::Vector2f const p0 { from.position + (settings.node_radius + is_double*arrow_offset) * out };
            sf::Vector2f const p3 { to.position   + (settings.node_radius +           arrow_offset) * in  };

            sf::Vector2f const p1 { p0 + link_rank * 2.0f*settings.node_radius * out };
            sf::Vector2f const p2 { p3 + link_rank * 2.0f*settings.node_radius * in };

            // calculate position of arrow tips
            sf::Vector2f const  in_tip { p3 - settings.arrow_length * in };
            sf::Vector2f const out_tip { p0 - settings.arrow_length * out };

            // approximate the number of segments needed
            sf::Vector2f const average { 0.3f*p0 + 0.15f*p1 + 0.15f*p2 + 0.3f*p3 };
            std::size_t const segment_count((length(average - p0) + length(p3 - average)) / 16.0f);

            std::vector<sf::Vector2f> const path { bezier_curve(p0, p1, p2, p3, segment_count) };

            if (is_double)
            {
                draw_path_double_arrow(target, path, out_tip, in_tip, settings.link_width, settings.background_color, settings.link_gap_size);
                draw_path_double_arrow(target, path, out_tip, in_tip, settings.link_width, settings.foreground_color, 0.0f);
            }
            else
            {
                draw_path_single_arrow(target, path, in_tip, settings.link_width, settings.background_color, settings.link_gap_size);
                draw_path_single_arrow(target, path, in_tip, settings.link_width, settings.foreground_color, 0.0f);
            }
        }

    private:
    
        std::vector<Diagram_Node> nodes { };
        Diagram_Settings settings { };

    };

}

void draw_diagram(sf::RenderTarget& target, std::vector<Node_Ptr> const& roots,
                  Diagram_Settings const& settings)
{
    Diagram diagram { roots, settings };
    diagram.draw(target);
}
