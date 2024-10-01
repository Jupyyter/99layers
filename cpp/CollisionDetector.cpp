#include "../hpp/libs.hpp"

CollisionDetector::CollisionInfo CollisionDetector::CollisionSide(const sf::FloatRect &bounds1, const sf::FloatRect &bounds2)
{
    // First, check if the rectangles actually intersect
    sf::FloatRect intersection;
    if (!bounds1.intersects(bounds2, intersection))
    {
        return CollisionInfo::None;
    }

    // Calculate the overlaps from bounds1's perspective
    float leftOverlap = (bounds1.left + bounds1.width) - bounds2.left;
    float rightOverlap = (bounds2.left + bounds2.width) - bounds1.left;
    float topOverlap = (bounds1.top + bounds1.height) - bounds2.top;
    float bottomOverlap = (bounds2.top + bounds2.height) - bounds1.top;

    // Find the smallest overlap
    float minOverlap = std::min({leftOverlap, rightOverlap, topOverlap, bottomOverlap});

    // Determine which side had the smallest overlap
    if (minOverlap == leftOverlap)
    {
        return CollisionInfo::Right; // bounds1's right side hit bounds2's left side
    }
    else if (minOverlap == rightOverlap)
    {
        return CollisionInfo::Left; // bounds1's left side hit bounds2's right side
    }
    else if (minOverlap == topOverlap)
    {
        return CollisionInfo::Bottom; // bounds1's bottom hit bounds2's top
    }
    else if (minOverlap == bottomOverlap)
    {
        return CollisionInfo::Top; // bounds1's top hit bounds2's bottom
    }

    return CollisionInfo::None; // Should never reach here if there's an intersection
}
