#pragma once
class Animation;
class CollisionDetector;
class GameMap;
class EditorMap;
class Object;

struct PropertyDescriptor
{
    std::string name;
    std::string defaultValue;
    std::function<void(Object *, const std::string &)> setter;
    std::function<std::string(const Object *)> getter;
};

class Object
{
    friend class GameMap;

public:
    Object(const sf::Vector2f &position);
    virtual ~Object() = default;

    virtual void update(float deltaTime, const sf::Vector2u &screenrese) = 0;
    virtual void draw(sf::RenderWindow &window) const = 0;
    virtual sf::FloatRect getBounds() const = 0;
    virtual const sf::Sprite getSprite() const = 0;
    virtual bool isOnScreen() const = 0;

    static std::vector<PropertyDescriptor> getPropertyDescriptors() { return {}; }

    sf::Vector2f getPosition() const { return position; }
    void move(const sf::Vector2f &offset) { position += offset; }
    int getPriorityLayer() { return priorityLayer; }

    int priorityLayer = 0;
    bool shouldBeDead = false;

private:
    virtual void setPosition(const sf::Vector2f &pos) { position = pos; }

protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
};