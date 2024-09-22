#pragma once
class Player;
class Entity;
class EntityFactory;
class TextBox;
class Item;

class GameMap
{
private:
    class Object
    {
    public:
        Object(int x, int y, int w, int h, std::string tname);
        ~Object() = default;

        void draw(sf::RenderWindow &window);

        sf::RectangleShape rect;
        sf::Texture tex;
        std::string texid;
    };

public:
    void spawn(std::string entityName, float x, float y,float rotation);
    void loadFromFile(const std::string& fname);
    void handleCollisions();
    GameMap(sf::RenderWindow &wndref, bool &gameover);
    GameMap(std::string fname, sf::RenderWindow &wndref, bool &gameover);
    GameMap(const GameMap &) = delete;
    GameMap &operator=(const GameMap &) = delete;
    ~GameMap();

    void draw();
    void changePart(int x, int y);
    sf::FloatRect getPartBounds();
    std::vector<sf::FloatRect> getObjectBounds();
    std::vector<sf::FloatRect> getEntityBounds();

    void resetEntities();
    void spawnEntities();
    void updateEntities(float deltaTime, const sf::Vector2u &windowSize);
    void drawEntities(sf::RenderWindow &window);

    std::vector<Entity *> activeEntities;
    std::vector<Item *> allItems;
    sf::FloatRect *playerBounds;

    bool *gameOver;
    sf::RenderWindow &wndref;
    std::vector<std::unique_ptr<Entity::PlacedEntity>> placedEntities;

private:
    void removeDeadEntities();

    std::unordered_map<int, std::unordered_map<int, std::vector<Object *>>> obj;
    int mx, my, np;
    sf::View view;
};