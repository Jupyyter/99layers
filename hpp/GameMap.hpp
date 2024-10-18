#pragma once
#include <vector>
#include <algorithm>
#include <memory>

class Player;
class Entity;
class EntityFactory;
class TextBox;
class Item;
class CollisionDetector;

class GameMap
{
public:
    void spawn(const std::string &entityName, float x = 0, float y = 0, float rotation = 0);
    void spawn(Entity *entity);
    void loadFromFile(const std::string &fname);
    GameMap(sf::RenderWindow &wndref, bool &gameover);
    GameMap(std::string fname, sf::RenderWindow &wndref, bool &gameover);
    GameMap(const GameMap &) = delete;
    GameMap &operator=(const GameMap &) = delete;
    ~GameMap() = default; // The destructor can now be defaulted

    void changePart(int x, int y);
    sf::FloatRect getPartBounds() const;

    void resetEntities();
    void updateEntities(float deltaTime, const sf::Vector2u &windowSize);
    void drawEntities(sf::RenderWindow &window) const;

    Player *playerRef;

    bool *gameOver;
    sf::RenderWindow &wndref;
    void removeDeadEntities();

private:
    struct EntityCompare
    {
        bool operator()(const std::unique_ptr<Entity> &a, const std::unique_ptr<Entity> &b) const { return a->priorityLayer < b->priorityLayer; }
    };
    std::vector<CollisionDetector *> collisionEntities; // This remains as raw pointers for performance
    std::vector<EditorMap::PlacedEntity> originalEntities;
    std::multiset<std::unique_ptr<Entity>, EntityCompare> allEntities;
    void spawnEntities();
    bool checkCollision(const sf::Sprite &sprite1, const sf::Sprite &sprite2);
    std::vector<sf::Vector2f> getTransformedBounds(const sf::Sprite &sprite);

    bool checkSATCollision(const std::vector<sf::Vector2f> &vertices1, const std::vector<sf::Vector2f> &vertices2);

    int mx, my, np;
    sf::View view;
};

// global world
inline std::unique_ptr<GameMap> world;