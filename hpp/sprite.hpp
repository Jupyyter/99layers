#pragma once
class Entity;
class Sprite : public Entity
{
public:
    virtual std::vector<std::pair<std::string, std::string>> getEditableProperties() const
    {
        return {};
    }
    virtual void setProperty(const std::string &name, const std::string &value)
    {
    }
    Sprite();
    void loadTexture(const std::string &filename);
    void update(float deltaTime, const sf::Vector2u &screenres) override;
    void draw(sf::RenderWindow &window) const override;
    sf::FloatRect getBounds() const override;
    const sf::Sprite getSprite()const override;
    void setPosition(const sf::Vector2f &pos);
    void setPosition(float x, float y);
    void scale(float scaleX, float scaleY); 
    sf::Texture texture;
    sf::Sprite sprite;
    bool invisible;
    bool isOnScreen() const override;

protected:
    bool flipped;
};