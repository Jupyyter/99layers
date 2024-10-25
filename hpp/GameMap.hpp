#pragma once

class Player;
class Object;
class TextBox;
class Item;
class CollisionDetector;
class Sprite;

class GameMap {
public:
    // Constructors & Destructors
    GameMap(sf::RenderWindow& wndref, bool& gameover);
    GameMap(std::string fname, sf::RenderWindow& wndref, bool& gameover);
    GameMap(const GameMap&) = delete;  // Prevent copying
    GameMap& operator=(const GameMap&) = delete;  // Prevent assignment
    ~GameMap() = default;

    // Object Management
    void spawn(const std::string& objectName, float x = 0, float y = 0, float rotation = 0);
    void spawn(Object* object);
    void removeDeadObjects();

    // Map Loading & Reset
    void loadFromFile(const std::string& fname);
    // Camera Control
    void resetCamera();
    void setCameraZoom(float zoom) { m_camera.setZoom(zoom); }
    void setCameraSmoothness(float smoothness) { m_camera.setSmoothness(smoothness); }
    float getCameraZoom() const { return m_camera.getZoom(); }
    const sf::Vector2f& getCameraPosition() const { return m_camera.getPosition(); }
    const sf::Vector2f& getCameraSize() const { return m_camera.getSize(); }

    // Part/Section Management
    void changePart(int x, int y, bool instant = false);
    void teleportToPart(int x, int y) { changePart(x, y, true); }
    sf::FloatRect getPartBounds() const;
    int getCurrentPartX() const { return m_currentPartX; }
    int getCurrentPartY() const { return m_currentPartY; }
    void teleportTo(int x, int y);
    void stopAllSounds();

    // Update & Draw
    void updateObjects(float deltaTime, const sf::Vector2u& windowSize);
    void drawObjects(sf::RenderWindow& window) const;

    // Public References
    Player* playerRef;
    bool* gameOver;
    sf::RenderWindow& wndref;
    std::vector<sf::Sound> gameSounds;

void spawnObjects();
    void deleteObjects();
private:
    // Internal Object Management
    struct ObjectCompare {
        bool operator()(const std::unique_ptr<Object>& a, const std::unique_ptr<Object>& b) const {
            return a->priorityLayer < b->priorityLayer;
        }
    };

    struct SpriteCompare {
        bool operator()(const Sprite* a, const Sprite* b) const {
            return a->priorityLayer < b->priorityLayer;
        }
    };

    std::multiset<std::unique_ptr<Object>, ObjectCompare> allObjects;
    std::vector<CollisionDetector*> collisionObjects;
    std::vector<Sprite*> visibleObjects;  // Sorted by priority layer
    std::vector<EditorMap::PlacedObject> originalObjects;

    // Camera Management
    Camera m_camera;
    int m_currentPartX;
    int m_currentPartY;

    // Internal Methods
    bool checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2);
    std::vector<sf::Vector2f> getTransformedBounds(const sf::Sprite& sprite);
    bool checkSATCollision(const std::vector<sf::Vector2f>& vertices1, 
                          const std::vector<sf::Vector2f>& vertices2);
};

// Global World Instance
inline std::unique_ptr<GameMap> world;