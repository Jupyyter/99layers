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