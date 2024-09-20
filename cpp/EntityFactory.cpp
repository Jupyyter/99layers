#include "../hpp/libs.hpp"
Entity *EntityFactory::createEntity(const std::string &type, const sf::Vector2f &position, Map &map, bool additemsToinventory)
{
    if (type == "pacman")
    {
        return new PacMan(position, *map.gameOver);
    }
    else if (type == "arrow")
    {
        return new Idk(position, 200);
    }
    else if (type == "capybaraa")
    {
        return new Npc(position);
    }
    else if (type == "pengu")
    {
        return new Penguin(position);
    }
    else if (type == "HorusBrogans")
    {
        HB *item = new HB(position);
        if (additemsToinventory)
        {
            map.allItems.push_back(item);
        }
        return item;
    }
    else if (type == "runnerspact")
    {
        RP *item = new RP(position);
        if (additemsToinventory)
        {
            map.allItems.push_back(item);
        }
        return item;
    }
    else if (type == "groundbreaker")
    {
        GB *item = new GB(position);
        if (additemsToinventory)
        {
            map.allItems.push_back(item);
        }
        return item;
    }
    else if (type == "chronostimepiece")
    {
        CTP *item = new CTP(position);
        if (additemsToinventory)
        {
            map.allItems.push_back(item);
        }
        return item;
    }
    else if(type=="poketIkeaman"){
        II *item = new II(position);
        if (additemsToinventory)
        {
            map.allItems.push_back(item);
        }
        return item;
    }
    return nullptr;
}