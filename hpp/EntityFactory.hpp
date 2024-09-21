#pragma once
#include "libs.hpp"
class PacMan;
class Player;
class Boss;
class Npc;
class Idk;

class EntityFactory {
public:
    static Entity* createEntity(const std::string& type, const sf::Transformable position , GameMap& gamemap,bool additemsToinventory=false);
    static Entity* createEntity(const std::string& type, const sf::Transformable position , EditorMap& editormap,bool additemsToinventory=false);
};