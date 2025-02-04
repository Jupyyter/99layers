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

    virtual void update(float deltaTime, const sf::Vector2u &screenrese) {};
    virtual bool isOnScreen() const = 0;

    static std::vector<PropertyDescriptor> getPropertyDescriptors() { return {}; }

    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getVelocity() const { return velocity; }

    void move(const sf::Vector2f &offset) { position += offset; }
    int getPriorityLayer() { return priorityLayer; }

    int priorityLayer = 0,id;
    bool shouldBeDead = false;
    std::string getName(){return name;}
    void setName(std::string const &nam){name=nam;}

private:
    std::string name;

protected:
    void setPosition(const sf::Vector2f &pos) { position = pos; }
    void setPositionX(const int &pos) { position.x = pos; }
    void setPositionY(const int &pos) { position.y = pos; }
    sf::Vector2f position;
    sf::Vector2f velocity;
};