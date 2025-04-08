#include "lib/diagram.h"

#include <thread>
#include <mutex>

#include <SFML/Graphics.hpp>

std::mutex redraw_mutex { };
bool should_redraw { false };

// Window and drawing utilities
namespace
{

    bool running { true };

    void redraw(sf::RenderWindow& window, std::vector<Node_Ptr> const& roots,
                       Diagram_Settings const& settings)
    {
        window.clear(settings.background_color);
        draw_diagram(window, roots, settings);
        window.display();
    }

    void configure_window(sf::RenderWindow& window, Diagram_Settings const& settings)
    {
        sf::ContextSettings context { };
        context.antialiasingLevel = 8;

        sf::VideoMode desktop { sf::VideoMode::getDesktopMode() };
        
        window.create(sf::VideoMode(0.5*desktop.width, 0.5*desktop.height),
                      "Directed Graph Diagram", sf::Style::Default, context);

        window.clear(settings.background_color);
        window.display();
    }

}

void notify_exit()
{
    running = false;
}

void run_window(std::vector<Node_Ptr> const& roots)
{
    Diagram_Settings settings { };

    sf::RenderWindow window { };
    configure_window(window, settings);

    while (running)
    {
        while (window.isOpen() && running)
        {
            sf::Event event { };
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                else if (event.type == sf::Event::Resized)
                {
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                    should_redraw = true;
                }
            }

            if (should_redraw)
            {
                std::lock_guard<std::mutex> const lock { redraw_mutex };
                redraw(window, roots, settings);
                should_redraw = false;
            }

        }

        if (should_redraw)
        {
            std::lock_guard<std::mutex> const lock { redraw_mutex };
            configure_window(window, settings);
            redraw(window, roots, settings);
            should_redraw = false;
        }
    }
}
