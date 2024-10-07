#pragma once
#include "libs.hpp"
class PacMan;
class Player;
class Boss;
class Npc;
class Idk;

class EntityFactory {
public:
    static Entity* createEntity(const std::string& type, const sf::Transformable position ,bool additemsToinventory=false);
    static std::vector<PropertyDescriptor> getPropertyDescriptors(const std::string& type);
};