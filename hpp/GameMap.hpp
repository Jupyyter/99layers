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
    void spawn(const std::string& objectName, float x = 0, float y = 0, float rotation = 0,sf::Vector2f vel=sf::Vector2f(0,0));
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
    bool isPlayerValid; //im sorry, i cant find a better/faster way than this
    std::vector<sf::Sound> gameSounds;

void setItemRespawnOff(int id);
void spawnObjects();
    void deleteObjects();
    sf::Vector2f playerSpawnPosition;

    void toggleFullscreen();
    void handleResize(const sf::Vector2u& newSize);
private:

sf::Vector2f m_originalResolution;
    sf::Vector2f m_viewportPosition;
    sf::Vector2f m_viewportSize;
    bool m_isFullscreen;

    void updateViewport();

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
    std::multiset<Sprite*, SpriteCompare> visibleObjects;  // Sorted by priority layer
    std::vector<EditorMap::PlacedObject> originalObjects;   // the ones placed in the editor

    // Camera Management
    Camera m_camera;
    int m_currentPartX;
    int m_currentPartY;

    // Internal Methods
    void performCollisionChecks(Object *object,bool lastCheck);
    bool checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2);
    std::vector<sf::Vector2f> getTransformedBounds(const sf::Sprite& sprite) const;
    bool checkSATCollision(const std::vector<sf::Vector2f>& vertices1, 
                          const std::vector<sf::Vector2f>& vertices2);

    // Add spatial partitioning grid
    static const int GRID_SIZE = 128; // Size of each grid cell
    std::vector<std::vector<std::vector<Sprite*>>> spatialGrid;
    sf::Vector2i gridDimensions;

    // Add view frustum for culling
    sf::FloatRect viewBounds;
    
    // Cache transformed bounds
    struct TransformedBounds {
        std::vector<sf::Vector2f> vertices;
        float timestamp;
    };
    mutable std::unordered_map<const Sprite*, TransformedBounds> boundsCache;
    
    // Add clock for cache timing
    mutable sf::Clock transformClock;
    
    // Add helper methods
    void updateViewBounds();
    std::vector<Sprite*> getNearbySprites(const Sprite* sprite) const;
    bool isInView(const Sprite* sprite) const;
    void clearCaches();
};

// Global World Instance
inline std::unique_ptr<GameMap> world;