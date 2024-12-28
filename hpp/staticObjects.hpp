#pragma once
class sign0 : public Sprite{
public:
    sign0(sf::Vector2f position);
void update(float deltaTime, const sf::Vector2u& screenres) override{};
    void draw(sf::RenderWindow &window) const override{window.draw(sprite);};

};
class sign1 : public Sprite{
public:
    sign1(sf::Vector2f position);
void update(float deltaTime, const sf::Vector2u& screenres) override{};
    void draw(sf::RenderWindow &window) const override{window.draw(sprite);};
    static std::vector<PropertyDescriptor> getPropertyDescriptors();
    bool what;
};
class sign2 : public Sprite{
public:
    sign2(sf::Vector2f position);
void update(float deltaTime, const sf::Vector2u& screenres) override{};
    void draw(sf::RenderWindow &window) const override{window.draw(sprite);};
private:
};