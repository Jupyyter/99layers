#pragma once
#include <vector>
#include <algorithm>
#include <memory>

class Player;
class Object;
class TextBox;
class Item;
class CollisionDetector;

class GameMap
{
public:
    void spawn(const std::string &objectName, float x = 0, float y = 0, float rotation = 0);
    void spawn(Object *object);
    void loadFromFile(const std::string &fname);
    GameMap(sf::RenderWindow &wndref, bool &gameover);
    GameMap(std::string fname, sf::RenderWindow &wndref, bool &gameover);
    GameMap(const GameMap &) = delete;
    GameMap &operator=(const GameMap &) = delete;
    ~GameMap() = default; // The destructor can now be defaulted

    void changePart(int x, int y);
    sf::FloatRect getPartBounds() const;

    void resetObjects();
    void updateObjects(float deltaTime, const sf::Vector2u &windowSize);
    void drawObjects(sf::RenderWindow &window) const;

    Player *playerRef;

    bool *gameOver;
    sf::RenderWindow &wndref;
    void removeDeadObjects();

private:
    struct ObjectCompare
    {
        bool operator()(const std::unique_ptr<Object> &a, const std::unique_ptr<Object> &b) const { return a->priorityLayer < b->priorityLayer; }
    };
    std::vector<CollisionDetector *> collisionObjects; // This remains as raw pointers for performance
    std::vector<EditorMap::PlacedObject> originalObjects;
    std::multiset<std::unique_ptr<Object>, ObjectCompare> allObjects;
    void spawnObjects();
    bool checkCollision(const sf::Sprite &sprite1, const sf::Sprite &sprite2);
    std::vector<sf::Vector2f> getTransformedBounds(const sf::Sprite &sprite);

    bool checkSATCollision(const std::vector<sf::Vector2f> &vertices1, const std::vector<sf::Vector2f> &vertices2);

    int mx, my, np;
    sf::View view;
};

// global world
inline std::unique_ptr<GameMap> world;