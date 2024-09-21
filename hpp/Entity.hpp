#pragma once
class Animation;
class CollisionDetector;
class GameMap;
class EditorMap;
class Entity {
public:
    Entity();
    virtual ~Entity() = default;
    
    virtual void update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenrese) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual std::vector<std::pair<std::string, std::string>> getEditableProperties() const{
        return {};
    }
    virtual void setProperty(const std::string& name, const std::string& value){
        
    }
    
    sf::Vector2f getPosition() const;
    void move(const sf::Vector2f& offset);
    bool isOnScreen(sf::FloatRect screen);
    int priorityLayer;

    bool shouldBeDead;

    struct PlacedEntity
    {
        sf::Sprite sprite;
        std::string type;
        std::unique_ptr<Entity> entity;
    };

    sf::Vector2f position;
protected:
    sf::Vector2f velocity;
};