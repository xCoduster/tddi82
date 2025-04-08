#include "lib/math.h"

float length(sf::Vector2f p)
{
    return std::sqrt(length_squared(p));
}

float length_squared(sf::Vector2f p)
{
    return p.x*p.x + p.y*p.y;
}

float direction(sf::Vector2f p)
{
    return std::atan2(p.y, p.x);
}

sf::Vector2f unit(float angle)
{
    return { std::cos(angle), std::sin(angle) };
}

sf::Vector2f normalize(sf::Vector2f p)
{
    return p / length(p);
}

sf::Vector2f normal(sf::Vector2f p)
{
    p = { -p.y, p.x };
    return normalize(p);
}

sf::Vector2f intersection(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f q0, sf::Vector2f q1)
{
    float const denom { (p0.x - p1.x)*(q0.y - q1.y) - (p0.y - p1.y)*(q0.x - q1.x) };

    float const K1 { (p0.x*p1.y - p0.y*p1.x) };
    float const K2 { (q0.x*q1.y - q0.y*q1.x) };
        
    float const x { K1*(q0.x - q1.x) - K2*(p0.x - p1.x) };
    float const y { K1*(q0.y - q1.y) - K2*(p0.y - p1.y) };

    return { x / denom, y / denom };
}

std::vector<sf::Vector2f> bezier_curve(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2,
                                       sf::Vector2f p3, std::size_t segments)
{
    std::vector<sf::Vector2f> path(segments + 1);
    for (std::size_t i { 0 }; i < path.size(); ++i)
    {
        float const t { static_cast<float>(i) / segments };
        float const t2 { t*t };
        float const t3 { t*t2 };

        float const it { 1.0f - t };
        float const it2 { it*it };
        float const it3 { it*it2 };

        path[i] = it3*p0 + 3.0f*it2*t*p1 + 3.0f*it*t2*p2 + t3*p3;
    }
    return path;
}

float signbit(bool bit)
{
    return 1.0f - 2.0f*bit;
}
