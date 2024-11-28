#pragma once
class Npc : public Animation, public CollisionDetector {
public:
    Npc(sf::Vector2f position, float frameInterval, const std::string &imgPath="../imgs/hammer.jpg");
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const override;
    static std::vector<PropertyDescriptor> getPropertyDescriptors();
    void onCollision(Sprite* other) override;

protected:
    float gravity;
    std::string text;
    TextBox* textBox;
    bool isInteracting;
    bool wasKeyPressed;
    int extraWidth,extraHeight;
    // virtual so derived classes can override the behavior
    virtual void handleTextBoxInput();
};