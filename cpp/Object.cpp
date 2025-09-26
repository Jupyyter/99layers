#include "../hpp/libs.hpp"

Object::Object(const sf::Vector2f &position) : position(0,0), velocity(0, 0),shouldBeDead(false),priorityLayer(0) {
    setPosition(position);
}

bool Object::isOnScreen() const {
    return world->getPartBounds().contains(position);
}