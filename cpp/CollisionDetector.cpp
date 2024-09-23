#include "../hpp/libs.hpp"

CollisionDetector::CollisionInfo CollisionDetector::checkCollision(const sf::FloatRect& bounds, const std::vector<sf::FloatRect>& obstacles) {
        CollisionInfo result{false, CollisionSide::None, sf::FloatRect()};
        float minOverlap = std::numeric_limits<float>::max();

        for (const auto& obstacle : obstacles) {
            sf::FloatRect intersection;
            if (bounds.intersects(obstacle, intersection)) {
                result.collided = true;
                
                float leftOverlap = intersection.left + intersection.width - obstacle.left;
                float rightOverlap = obstacle.left + obstacle.width - intersection.left;
                float topOverlap = intersection.top + intersection.height - obstacle.top;
                float bottomOverlap = obstacle.top + obstacle.height - intersection.top;

                float currentMinOverlap = std::min({leftOverlap, rightOverlap, topOverlap, bottomOverlap});

                if (currentMinOverlap < minOverlap) {
                    minOverlap = currentMinOverlap;
                    result.intersection = intersection;

                    if (minOverlap == leftOverlap) result.side = CollisionSide::Left;
                    else if (minOverlap == rightOverlap) result.side = CollisionSide::Right;
                    else if (minOverlap == topOverlap) result.side = CollisionSide::Bottom;
                    else if (minOverlap == bottomOverlap) result.side = CollisionSide::Top;
                }
            }
        }

        return result;
    }


static bool pointInRect(const sf::Vector2f& point, const sf::FloatRect& rect) {
        return point.x >= rect.left && point.x <= rect.left + rect.width &&
               point.y >= rect.top && point.y <= rect.top + rect.height;
    }