#pragma once
class Animation;
class CollisionDetector;
class GameMap;
class EditorMap;
class Entity;

struct PropertyDescriptor
{
    std::string name;
    std::string defaultValue;
    std::function<void(Entity *, const std::string &)> setter;
    std::function<std::string(const Entity *)> getter;
};

class Entity
{
public:
    Entity();
    virtual ~Entity() = default;

    virtual void update(float deltaTime, const sf::Vector2u &screenrese) = 0;
    virtual void draw(sf::RenderWindow &window) const = 0;
    virtual void onCollision(Entity *other) {}
    virtual sf::FloatRect getBounds() const = 0;
    virtual const sf::Sprite getSprite() const = 0;

    static std::vector<PropertyDescriptor> getPropertyDescriptors() { return {}; }

    sf::Vector2f getPosition() const { return position; }
    void setPosition(const sf::Vector2f &pos) { position = pos; }
    void move(const sf::Vector2f &offset) { position += offset; }
    bool isOnScreen(sf::FloatRect screen);

    int priorityLayer = 0;
    bool shouldBeDead = false;
    sf::Vector2f position;

protected:
    sf::Vector2f velocity;
};