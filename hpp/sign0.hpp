#pragma once
class sign0 : public Sprite{
public:
    sign0(sf::Vector2f position);
private:
    void loadSprite();
void update(float deltaTime, const sf::Vector2u& screenres) override{};
    void draw(sf::RenderWindow &window) const override{window.draw(sprite);};

};