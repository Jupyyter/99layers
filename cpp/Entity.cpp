#include "../hpp/libs.hpp"

Entity::Entity() : position(0,0), velocity(0, 0),shouldBeDead(false),priorityLayer(0) {}

bool Entity::isOnScreen() const {
    return world->getPartBounds().contains(position);
}