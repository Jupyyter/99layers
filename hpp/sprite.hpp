#pragma once
class Object;
class Sprite : public Object
{
public:
    virtual std::vector<std::pair<std::string, std::string>> getEditableProperties() const
    {
        return {};
    }
    virtual void setProperty(const std::string &name, const std::string &value)
    {
    }
    Sprite(const sf::Vector2f& position);
    void loadTexture(const std::string &filename);
    void update(float deltaTime, const sf::Vector2u &screenres) override;
    void draw(sf::RenderWindow &window) const override;
    sf::FloatRect getBounds() const override;
    const sf::Sprite getSprite()const override;
    void scale(float scaleX, float scaleY); 
    sf::Texture texture;
    sf::Sprite sprite;
    bool invisible;
    bool isOnScreen() const override;

private:
    void setPosition(const sf::Vector2f &pos) override;
protected:
    bool flipped;
};