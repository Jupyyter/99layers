#include "../hpp/libs.hpp"
Entity *EntityFactory::createEntity(const std::string &type,sf::Transformable transform, GameMap &gamemap, bool additemsToinventory)
{
    if (type == "pacman")
    {
        return new PacMan(transform.getPosition(), *gamemap.gameOver);
    }
    else if (type == "arrow")
    {
        return new Idk(transform.getPosition(), 200);
    }
    else if (type == "capybaraa")
    {
        return new Npc(transform.getPosition());
    }
    else if (type == "pengu")
    {
        return new Penguin(transform.getPosition());
    }
    else if (type == "HorusBrogans")
    {
        HB *item = new HB(transform.getPosition());
        if (additemsToinventory)
        {
            gamemap.allItems.push_back(item);
        }
        return item;
    }
    else if (type == "runnerspact")
    {
        RP *item = new RP(transform.getPosition());
        if (additemsToinventory)
        {
            gamemap.allItems.push_back(item);
        }
        return item;
    }
    else if (type == "groundbreaker")
    {
        GB *item = new GB(transform.getPosition());
        if (additemsToinventory)
        {
            gamemap.allItems.push_back(item);
        }
        return item;
    }
    else if (type == "chronostimepiece")
    {
        CTP *item = new CTP(transform.getPosition());
        if (additemsToinventory)
        {
            gamemap.allItems.push_back(item);
        }
        return item;
    }
    else if(type=="poketIkeaman"){
        II *item = new II(transform.getPosition());
        if (additemsToinventory)
        {
            gamemap.allItems.push_back(item);
        }
        return item;
    }
    else if(type=="laser"){
        return new LaserBeam(transform.getPosition(),transform.getRotation());
    }
    else if(type=="plank"){
        return new Plank(transform.getPosition());
    }
    else if(type=="table"){
        return new TableFall(transform.getPosition());
    }
    return nullptr;
}
Entity *EntityFactory::createEntity(const std::string &type, sf::Transformable transform, EditorMap &editormap, bool additemsToinventory)
{
    if (type == "pacman")
    {
        return new PacMan(transform.getPosition(), *editormap.gameOver);
    }
    else if (type == "arrow")
    {
        return new Idk(transform.getPosition(), 200);
    }
    else if (type == "capybaraa")
    {
        return new Npc(transform.getPosition());
    }
    else if (type == "pengu")
    {
        return new Penguin(transform.getPosition());
    }
    else if (type == "HorusBrogans")
    {
        HB *item = new HB(transform.getPosition());
        if (additemsToinventory)
        {
            editormap.allItems.push_back(item);
        }
        return item;
    }
    else if (type == "runnerspact")
    {
        RP *item = new RP(transform.getPosition());
        if (additemsToinventory)
        {
            editormap.allItems.push_back(item);
        }
        return item;
    }
    else if (type == "groundbreaker")
    {
        GB *item = new GB(transform.getPosition());
        if (additemsToinventory)
        {
            editormap.allItems.push_back(item);
        }
        return item;
    }
    else if (type == "chronostimepiece")
    {
        CTP *item = new CTP(transform.getPosition());
        if (additemsToinventory)
        {
            editormap.allItems.push_back(item);
        }
        return item;
    }
    else if(type=="poketIkeaman"){
        II *item = new II(transform.getPosition());
        if (additemsToinventory)
        {
            editormap.allItems.push_back(item);
        }
        return item;
    }
    return nullptr;
}