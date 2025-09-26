#pragma once
class InteractiveObject : public CollisionDetector
{
public:
    InteractiveObject(const std::string &imgPath);
    virtual ~InteractiveObject() = default;

    // Common functionality for interactive objects
    virtual void update(float deltaTime, const sf::Vector2u &screenres);
    void onCollision(Sprite *other) override;

protected:
    TextBox *textBox;
    bool isInteracting;
    std::string text;

    // Virtual functions that derived classes should implement
    virtual void onInteractionStart()
    {
        textBox->resetState();
        textBox->setString(text);
    };
    virtual void onInteractionEnd() { textBox->setString(""); };
    virtual void updateInteraction(float deltaTime) = 0;
    virtual bool shouldEndInteraction() const = 0;
};