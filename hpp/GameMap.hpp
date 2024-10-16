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
    void spawn(Entity* entity);
    void loadFromFile(const std::string &fname);
    GameMap(sf::RenderWindow &wndref, bool &gameover);
    GameMap(std::string fname, sf::RenderWindow &wndref, bool &gameover);
    GameMap(const GameMap &) = delete;
    GameMap &operator=(const GameMap &) = delete;
    ~GameMap() = default; // The destructor can now be defaulted

    void draw() const;
    void changePart(int x, int y);
    sf::FloatRect getPartBounds() const;

    void resetEntities();
    void updateEntities(float deltaTime, const sf::Vector2u &windowSize);
    void drawEntities(sf::RenderWindow &window) const;

    Player* playerRef;

    bool *gameOver;
    sf::RenderWindow &wndref;
    void removeDeadEntities();

private:
struct EntityCompare {
        bool operator()(const std::unique_ptr<Entity>& a, const std::unique_ptr<Entity>& b) const {
            return a->priorityLayer < b->priorityLayer;
        }
    };
    std::vector<CollisionDetector*> collisionEntities; // This remains as raw pointers for performance
    std::vector<EditorMap::PlacedEntity> originalEntities;
    std::multiset<std::unique_ptr<Entity>, EntityCompare> allEntities;
void spawnEntities();
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


    int mx, my, np;
    sf::View view;
};

// global world
inline std::unique_ptr<GameMap> world;