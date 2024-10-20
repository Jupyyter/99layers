#include "../hpp/libs.hpp"

Entity::Entity(const sf::Vector2f &position) : position(0,0), velocity(0, 0),shouldBeDead(false),priorityLayer(0) {
    setPosition(position);
}

bool Entity::isOnScreen() const {
    return world->getPartBounds().contains(position);
}