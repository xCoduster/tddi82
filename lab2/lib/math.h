#ifndef LIB_MATH_H_
#define LIB_MATH_H_

#include <cmath>

#include <SFML/Graphics.hpp>

// calculate the length of the vector `p`
float length(sf::Vector2f p);

// calculate the squared length of the vector `p`
float length_squared(sf::Vector2f p);

// calculate the angle of the vector `p` (in radians)
float direction(sf::Vector2f p);

// create a vector of length one in the direction determined by `angle` (in radians)
sf::Vector2f unit(float angle);

// calculate the unit vector of `p` (i.e. a vector with the same direction,
// but with length one instead)
sf::Vector2f normalize(sf::Vector2f p);

// calculate the normal vector of `p` (i.e. the unit vector of `p` rotated
// 90 degrees counter-clockwise)
sf::Vector2f normal(sf::Vector2f p);

// find the intersection point of the two lines p (p0 -> p1) and q (q0 -> q1)
// Note: it is assumed that the lines p and q are NOT parallell and that they are infinite lines
sf::Vector2f intersection(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f q0, sf::Vector2f q1);

// generate a path of points which traces out a cubic Bézier curve.
std::vector<sf::Vector2f> bezier_curve(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2,
                                       sf::Vector2f p3, std::size_t segments);

// return -1.0f if `signbit` is true, and +1.0f if `signbit` is false
float signbit(bool bit);

constexpr float const pi { 3.141592653589f };

#endif
