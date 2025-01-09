#pragma once
class Object;
class Sprite : public Object
{
    friend class GameMap;
    friend class CollisionDetector;

public:
    virtual std::vector<std::pair<std::string, std::string>> getEditableProperties() const
    {
        return {};
    }
    virtual void setProperty(const std::string &name, const std::string &value)
    {
    }

    virtual void update(float deltaTime, const sf::Vector2u &screenrese) = 0;
    virtual void draw(sf::RenderWindow &window) const;
    virtual sf::FloatRect getBounds() const;
    virtual const sf::Sprite getSprite() const;
    virtual bool isOnScreen() const;
    Sprite(const sf::Vector2f &position);
    void loadTexture(const std::string &filename);
    void scale(float scaleX, float scaleY);
    sf::Texture texture;
    sf::Sprite sprite;
    bool invisible;

private:
    void updateSprite();

protected:
    bool flipped;
};