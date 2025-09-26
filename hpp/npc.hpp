#pragma once

class Npc : public Animation, public InteractiveObject {
public:
    Npc(sf::Vector2f position, float frameInterval, const std::string &imgPath="../imgs/hammer.jpg");
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const override;
    static std::vector<PropertyDescriptor> getPropertyDescriptors();

protected:
    float gravity;
    bool wasKeyPressed;
    int extraWidth, extraHeight;

    // InteractiveObject override implementations
    void updateInteraction(float deltaTime) override;
    bool shouldEndInteraction() const override;

    virtual void handleTextBoxInput();
};