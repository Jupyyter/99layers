#pragma once
class Object;
class CollisionDetector {
public:
    // Enum to represent the side of collision
    enum class CollisionInfo {
        None,
        Top,
        Bottom,
        Left,
        Right
    };
    // Check for collision between an object and obstacles
    CollisionInfo CollisionSide(const sf::FloatRect& bounds1, const sf::FloatRect& bounds2);
    virtual void onCollision(Object *other){};

};