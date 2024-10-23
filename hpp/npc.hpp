#pragma once
class Npc : public Animation, public CollisionDetector {
public:
    Npc(sf::Vector2f position,float frameInterval,const std::string &imgPath="../imgs/hammer.jpg");
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const override;
    static std::vector<PropertyDescriptor> getPropertyDescriptors();
    void onCollision(Object* other) override;

private:
    float gravity;
    std::string text;
    TextBox* textBox;
    bool isInteracting;
};