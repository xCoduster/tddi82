#ifndef LIB_GFX_H_
#define LIB_GFX_H_

#include <vector>

#include <SFML/Graphics.hpp>

// draw a straight line with some thickness
void draw_line(sf::RenderTarget& target, sf::Vector2f from, sf::Vector2f to,
               sf::Color color = sf::Color::White, float thickness = 1.0f);

// draw a path of points as different line segments
void draw_path(sf::RenderTarget& target, std::vector<sf::Vector2f> const& path,
               sf::Color color = sf::Color::White, float thickness = 1.0f);

// draw an arrow tip. The size of the arrow depends on the distance between
// `base` and `tip`. Note that the thickness determines how much of the arrow
// should be protruded, if thickness == 0.0f then the arrow tip is a sharp point.
void draw_arrow_tip(sf::RenderTarget& target, sf::Vector2f base, sf::Vector2f tip,
                    sf::Color color = sf::Color::White, float thickness = 0.0f);

// draw the path, with an arrow tip at the end (the tip points at the final
// point of `path`).
//
// `arrow_size` is the length of the arrow, `line_thickness` determines how
// thick the line is and `thickness` is an outline added to the whole figure.
//
void draw_path_single_arrow(sf::RenderTarget& target, std::vector<sf::Vector2f> const& path,
                            sf::Vector2f tip, float line_thickness = 1.0f,
                            sf::Color color = sf::Color::White, float thickness = 0.0f);

// similar to `draw_path_single_arrow` but adds an arrow tip att the beginning
// of the path too.
void draw_path_double_arrow(sf::RenderTarget& target, std::vector<sf::Vector2f> const& path,
                            sf::Vector2f begin_tip, sf::Vector2f end_tip,
                            float line_thickness = 1.0f,
                            sf::Color color = sf::Color::White, float thickness = 0.0f);

#endif
