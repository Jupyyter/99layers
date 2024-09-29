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

        void draw(sf::RenderWindow &window)const;

        sf::RectangleShape rect;
        sf::Texture tex;
        std::string texid;
    };

public:
bool checkPixelPerfectCollision( sf::Sprite s1,  sf::Sprite s2,int step = 4);
    void spawn(const std::string& entityName, float x=0, float y=0, float rotation=0);
    void spawn(Entity* entity);
    void loadFromFile(const std::string& fname);
    GameMap(sf::RenderWindow &wndref, bool &gameover);
    GameMap(std::string fname, sf::RenderWindow &wndref, bool &gameover);
    GameMap(const GameMap &) = delete;
    GameMap &operator=(const GameMap &) = delete;
    ~GameMap();

    void draw()const;
    void changePart(int x, int y);
    sf::FloatRect getPartBounds() const;
    std::vector<sf::FloatRect> getObjectBounds() const;

    void resetEntities();
    void spawnEntities();
    void updateEntities(float deltaTime, const sf::Vector2u &windowSize);
    void drawEntities(sf::RenderWindow &window)const;

    std::vector<Entity *> activeEntities;
    std::vector<Item *> allItems;
    Player* playerRef;

    bool *gameOver;
    sf::RenderWindow &wndref;
    std::vector<std::unique_ptr<Entity::PlacedEntity>> placedEntities;
void removeDeadEntities();
private:
    static bool checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
        const auto& bounds1 = getTransformedBounds(sprite1);
        const auto& bounds2 = getTransformedBounds(sprite2);

        return checkSATCollision(bounds1, bounds2) && checkSATCollision(bounds2, bounds1);
    }
    static std::vector<sf::Vector2f> getTransformedBounds(const sf::Sprite& sprite) {
        const auto& transform = sprite.getTransform();
        const auto& localBounds = sprite.getLocalBounds();

        std::vector<sf::Vector2f> vertices = {
            transform.transformPoint(0, 0),
            transform.transformPoint(localBounds.width, 0),
            transform.transformPoint(localBounds.width, localBounds.height),
            transform.transformPoint(0, localBounds.height)
        };

        return vertices;
    }

    static bool checkSATCollision(const std::vector<sf::Vector2f>& vertices1, const std::vector<sf::Vector2f>& vertices2) {
        for (size_t i = 0; i < vertices1.size(); ++i) {
            sf::Vector2f edge = vertices1[(i + 1) % vertices1.size()] - vertices1[i];
            sf::Vector2f axis(-edge.y, edge.x);
            float axisLengthSquared = axis.x * axis.x + axis.y * axis.y;

            float min1 = std::numeric_limits<float>::max();
            float max1 = std::numeric_limits<float>::lowest();
            for (const auto& vertex : vertices1) {
                float projection = (vertex.x * axis.x + vertex.y * axis.y) / axisLengthSquared;
                min1 = std::min(min1, projection);
                max1 = std::max(max1, projection);
            }

            float min2 = std::numeric_limits<float>::max();
            float max2 = std::numeric_limits<float>::lowest();
            for (const auto& vertex : vertices2) {
                float projection = (vertex.x * axis.x + vertex.y * axis.y) / axisLengthSquared;
                min2 = std::min(min2, projection);
                max2 = std::max(max2, projection);
            }

            if (max1 < min2 || max2 < min1) {
                return false;
            }
        }

        return true;
    }
    std::unordered_map<int, std::unordered_map<int, std::vector<std::unique_ptr<Object>>>> obj;
    int mx, my, np;
    sf::View view;
};