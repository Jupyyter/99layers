#pragma once
#include "libs.hpp"
class PacMan;
class Player;
class Boss;
class Npc;
class Idk;

class ObjectFactory {
public:
    static Object* createObject(const std::string& type, const sf::Transformable position);
    static std::vector<PropertyDescriptor> getPropertyDescriptors(const std::string& type);
};