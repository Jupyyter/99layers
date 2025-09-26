#pragma once
class woodSign : public Npc{
public:
    woodSign(sf::Vector2f position);
private:
    void loadSprite();
};

class Capybara : public Npc{
public:
    Capybara(sf::Vector2f position);
private:
    void loadSprite();
};

class crow : public Npc{
public:
    crow(sf::Vector2f position);
private:
    void loadSprite();
};

class crowMan : public Npc{
public:
    crowMan(sf::Vector2f position);
private:
    void loadSprite();
};

class duck : public Npc{
public:
    duck(sf::Vector2f position);
private:
    void loadSprite();
};

class chicken : public Npc{
public:
    chicken(sf::Vector2f position);
private:
    void loadSprite();
};