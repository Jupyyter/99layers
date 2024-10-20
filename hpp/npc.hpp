#pragma once
class Npc : public Animation, public CollisionDetector {
public:
    Npc(sf::Vector2f position);
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const  override;
     static std::vector<PropertyDescriptor> getPropertyDescriptors();
    std::unique_ptr<TextBox> textBox;
void onCollision(Object* other) override;
private:
    void loadSprite();
    void checkCollisionWithPlayer(const sf::FloatRect& playerBounds);
    float gravity;
    sf::RenderWindow* window;
    bool isColliding;
    std::string customString;
    std::string text;
};