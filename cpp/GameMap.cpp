#include "../hpp/libs.hpp"
class EditorMap;

GameMap::GameMap(sf::RenderWindow &wndref, bool &gameover)
    : wndref(wndref), gameOver(&gameover), m_camera(wndref), m_currentPartX(0), m_currentPartY(0), playerRef(nullptr), isPlayerValid(true), playerSpawnPosition(sf::Vector2f(0, 0))
{
    // Initialize the camera
    m_camera.setSize(sf::Vector2f(wndref.getSize()));
    m_camera.setPosition(sf::Vector2f(wndref.getSize().x / 2.f, wndref.getSize().y / 2.f));
    m_camera.setZoom(1.0f);
    m_camera.setSmoothness(25); // smoothness

    // Other initializations
    collisionObjects.clear();
    allObjects.clear();
}

GameMap::GameMap(std::string fname, sf::RenderWindow &wndref, bool &gameover)
    : wndref(wndref), gameOver(&gameover), m_camera(wndref), m_currentPartX(0), m_currentPartY(0), playerRef(nullptr)
{
    // Initialize the camera
    m_camera.setSize(sf::Vector2f(wndref.getSize()));
    m_camera.setPosition(sf::Vector2f(wndref.getSize().x / 2.f, wndref.getSize().y / 2.f));
    m_camera.setZoom(1.0f);
    m_camera.setSmoothness(25.0f); // smoothness

    // Load map from file
    loadFromFile(fname);

    // Other initializations
    collisionObjects.clear();
    allObjects.clear();
}

void GameMap::loadFromFile(const std::string &fname)
{
    std::ifstream file(fname, std::ios::binary);
    if (!file)
    {
        std::cerr << "\nFailed to open file for reading: " << fname;
        return;
    }

    // Clear existing objects
    originalObjects.clear();

    int objectCount;
    file.read(reinterpret_cast<char *>(&objectCount), sizeof(int));

    for (int i = 0; i < objectCount; ++i)
    {
        EditorMap::PlacedObject object;

        // Read object type
        int typeLength;
        file.read(reinterpret_cast<char *>(&typeLength), sizeof(int));
        object.type.resize(typeLength);
        file.read(&object.type[0], typeLength);

        // Read position and size
        float x, y, width, height;
        file.read(reinterpret_cast<char *>(&x), sizeof(float));
        file.read(reinterpret_cast<char *>(&y), sizeof(float));
        file.read(reinterpret_cast<char *>(&width), sizeof(float));
        file.read(reinterpret_cast<char *>(&height), sizeof(float));

        // Read texture path
        int texturePathLength;
        file.read(reinterpret_cast<char *>(&texturePathLength), sizeof(int));
        object.texturePath.resize(texturePathLength);
        file.read(&object.texturePath[0], texturePathLength);

        // Set up the sprite
        object.sprite.setPosition(x, y);
        sf::Texture texture;
        std::string fullTexturePath = object.texturePath;
        if (texture.loadFromFile(fullTexturePath))
        {
            object.sprite.setTexture(texture);
            if (object.type == "Terrain")
            {
                object.sprite.setTextureRect(sf::IntRect(0, 0, width, height));
            }
            else
            {
                float scaleX = width / texture.getSize().x;
                float scaleY = height / texture.getSize().y;
                object.sprite.setScale(scaleX, scaleY);
            }
        }

        // Read properties
        int propertyCount;
        file.read(reinterpret_cast<char *>(&propertyCount), sizeof(int));
        for (int j = 0; j < propertyCount; ++j)
        {
            int nameLength, valueLength;
            file.read(reinterpret_cast<char *>(&nameLength), sizeof(int));
            std::string name(nameLength, '\0');
            file.read(&name[0], nameLength);

            file.read(reinterpret_cast<char *>(&valueLength), sizeof(int));
            std::string value(valueLength, '\0');
            file.read(&value[0], valueLength);
            object.properties[name] = value;
        }

        originalObjects.push_back(std::move(object));
    }
}
void GameMap::stopAllSounds()
{
    // Stop all game-related sounds
    for (auto &sound : gameSounds)
    {
        sound.stop();
    }
    // gameMusic.stop();
}
void GameMap::changePart(int x, int y, bool teleport)
{
    m_currentPartX += x;
    m_currentPartY += y;
    m_camera.moveToNextPart(x, y, teleport);
}

void GameMap::teleportTo(int x, int y)
{
    m_currentPartX = x;
    m_currentPartY = y;
    sf::Vector2f newPosition(x * m_camera.getSize().x, y * m_camera.getSize().y);
    m_camera.setPosition(newPosition, true);
}

sf::FloatRect GameMap::getPartBounds() const
{
    const auto &size = m_camera.getSize();
    return sf::FloatRect(m_currentPartX * size.x, m_currentPartY * size.y, size.x, size.y);
}
void GameMap::deleteObjects()
{
    resetCamera();
    collisionObjects.clear();
    visibleObjects.clear();
    allObjects.clear();
}
void GameMap::resetCamera()
{
    m_currentPartX = 0;
    m_currentPartY = 0;
    m_camera.setPosition(sf::Vector2f(m_camera.getSize().x / 2.f, m_camera.getSize().y / 2.f), true);
}
void GameMap::setItemRespawnOff(int id)
{
    // Iterate through originalObjects to find the object with the matching id
    for (auto &placedObject : originalObjects)
    {
        if (placedObject.id == id)
        {
            // Set respawn to false for this object
            placedObject.respawn = false;
            return; // Exit after finding and modifying the object
        }
    }
}
void GameMap::spawnObjects()
{
    int q = 0;
    for (auto &placedObject : originalObjects)
    {
        sf::Vector2f objectPos = placedObject.sprite.getPosition();
        sf::Vector2f objectSize = placedObject.sprite.getGlobalBounds().getSize();
        Object *newObject = nullptr;

        if (placedObject.type == "Background")
        {
            // Create a Background object
            newObject = new Background(
                objectPos.x,
                objectPos.y,
                objectSize.x,
                objectSize.y,
                placedObject.texturePath);
        }
        else if (placedObject.type == "Terrain")
        {
            newObject = new Terrain(
                static_cast<int>(objectPos.x),
                static_cast<int>(objectPos.y),
                static_cast<int>(objectSize.x),
                static_cast<int>(objectSize.y),
                placedObject.texturePath);
            newObject->setName("Terrain");
        }
        else
        {
            sf::Transformable transform;
            transform.setPosition(placedObject.sprite.getPosition());
            transform.setScale(placedObject.sprite.getScale());
            newObject = ObjectFactory::createObject(placedObject.type, transform);
            Item *itm = dynamic_cast<Item *>(newObject);
            if (!placedObject.respawn && itm)
            {
                itm->position = world->playerRef->position;
            }
        }

        if (newObject)
        {
            // Set object properties
            auto descriptors = ObjectFactory::getPropertyDescriptors(placedObject.type);
            for (const auto &desc : descriptors)
            {
                auto it = placedObject.properties.find(desc.name);
                if (it != placedObject.properties.end() && desc.setter)
                {
                    desc.setter(newObject, it->second);
                }
            }
            newObject->id = q;
            placedObject.id = q;
            spawn(newObject);
        }
        q++;
    }
}
void GameMap::updateViewBounds()
{
    sf::Vector2f viewCenter = m_camera.getPosition();
    sf::Vector2f viewSize = m_camera.getSize();
    float zoom = m_camera.getZoom();

    viewBounds = sf::FloatRect(
        viewCenter.x - (viewSize.x / 2.f) / zoom,
        viewCenter.y - (viewSize.y / 2.f) / zoom,
        viewSize.x / zoom,
        viewSize.y / zoom);
}
bool GameMap::isInView(const Sprite *sprite) const
{
    return sprite->getBounds().intersects(viewBounds);
}

void GameMap::clearCaches()
{
    boundsCache.clear();
}

void GameMap::updateObjects(float deltaTime, const sf::Vector2u &windowSize)
{
    const float MAX_SPEED_THRESHOLD = 200.0f; // pixels per second
    const int MAX_SUBSTEPS = 15;              // Prevent excessive subdivision

    // Update camera and apply view
    m_camera.update(deltaTime);
    m_camera.applyTo(wndref);

    // Update view bounds for culling
    updateViewBounds();

    // First, update all objects to set their velocities
    for (const auto &object : allObjects)
    {
        object->update(deltaTime, windowSize);
    }

    // Then handle movement with potential substeps
    for (const auto &object : allObjects)
    {
        float objectSpeed = std::sqrt(object->getVelocity().x * object->getVelocity().x +
                                      object->getVelocity().y * object->getVelocity().y);
        if (objectSpeed > MAX_SPEED_THRESHOLD)
        {
            // Break movement into smaller substeps
            int substeps = std::min(
                static_cast<int>(std::ceil(objectSpeed / MAX_SPEED_THRESHOLD)),
                MAX_SUBSTEPS);
            float smallDeltaTime = deltaTime / substeps;

            sf::Vector2f originalPosition = object->getPosition();
            sf::Vector2f velocity = object->getVelocity();

            for (int i = 0; i < substeps; ++i)
            {
                sf::Vector2f newPosition = originalPosition + velocity * smallDeltaTime * (i + 1.f);
                object->setPosition(newPosition);
                if (i == substeps - 1)
                {
                    performCollisionChecks(object.get(), true);
                }
                else
                {
                    // Perform collision checks at each substep
                    performCollisionChecks(object.get(), false);
                }
            }
        }
        else if (objectSpeed != 0)
        {
            // Normal movement for slower objects
            object->setPosition(object->getPosition() + object->getVelocity() * deltaTime);
            performCollisionChecks(object.get(), true);
        }

        // Update sprite if it's a Sprite object
        Sprite *en1 = dynamic_cast<Sprite *>(object.get());
        if (en1)
        {
            en1->updateSprite();
        }
    }
}
void GameMap::performCollisionChecks(Object *object, bool lastCheck)
{
    Sprite *en1 = dynamic_cast<Sprite *>(object);
    if (en1 && en1->isOnScreen())
    {
        for (size_t j = 1; j < collisionObjects.size(); ++j)
        {
            Sprite *en2 = dynamic_cast<Sprite *>(collisionObjects[j]);
            if (dynamic_cast<CollisionDetector *>(en1) && en2 && en1 != en2)
            {
                // Quick AABB check before detailed collision
                sf::FloatRect bounds1 = en1->getBounds();
                sf::FloatRect bounds2 = en2->getBounds();
                if (bounds1.intersects(bounds2))
                {
                    // Detailed collision check
                    if (checkCollision(en1->getSprite(), en2->getSprite()))
                    {
                        if (lastCheck)
                        {
                            dynamic_cast<CollisionDetector *>(en2)->onCollision(en1);
                            dynamic_cast<CollisionDetector *>(en1)->onCollision(en2);
                        }
                        dynamic_cast<CollisionDetector *>(en1)->collide(en2);
                        dynamic_cast<CollisionDetector *>(en2)->collide(en1);
                    }
                }
                en1->updateSprite();
            }
        }
    }
}
void GameMap::removeDeadObjects()
{
    // Clear caches for dead objects
    for (auto it = boundsCache.begin(); it != boundsCache.end();)
    {
        if (it->first->shouldBeDead)
        {
            it = boundsCache.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Remove dead sprites from visibleObjects
    for (auto it = visibleObjects.begin(); it != visibleObjects.end();)
    {
        if ((*it)->shouldBeDead)
        {
            it = visibleObjects.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Rest of the removal code remains the same...
    for (auto it = allObjects.begin(); it != allObjects.end();)
    {
        if ((*it)->shouldBeDead)
        {
            it = allObjects.erase(it);
        }
        else
        {
            ++it;
        }
    }

    collisionObjects.erase(
        std::remove_if(collisionObjects.begin(), collisionObjects.end(),
                       [this](CollisionDetector *collisionDetector)
                       {
                           if (!collisionDetector)
                               return true;
                           return std::none_of(allObjects.begin(), allObjects.end(),
                                               [collisionDetector](const std::unique_ptr<Object> &object)
                                               {
                                                   return object.get() == dynamic_cast<Object *>(collisionDetector);
                                               });
                       }),
        collisionObjects.end());
}
void GameMap::drawObjects(sf::RenderWindow &window) const
{
    // Only draw visible objects
    for (const auto *sprite : visibleObjects)
    {
        sprite->draw(window);
    }
}

void GameMap::spawn(const std::string &objectName, float x, float y, float rotation,sf::Vector2f vel)
{
    sf::Transformable transform;
    transform.setPosition(x, y);
    transform.setRotation(rotation);
    Object *object = ObjectFactory::createObject(objectName, transform);
    if (object)
    {
        if(vel!=sf::Vector2f(0,0)){
            object->velocity=vel;
        }
        spawn(object);
    }
}
void GameMap::spawn(Object *object)
{
    try
    {
        // Add to main objects container
        allObjects.emplace(std::unique_ptr<Object>(object));

        // Check if it's a collision object
        CollisionDetector *collider = dynamic_cast<CollisionDetector *>(object);
        if (collider)
        {
            collisionObjects.push_back(collider);
        }

        // If it's a sprite, add to visible objects list
        Sprite *sprite = dynamic_cast<Sprite *>(object);
        if (sprite)
        {
            // No need to find position manually - multiset will sort automatically
            visibleObjects.insert(sprite);
        }
    }
    catch (const std::bad_alloc &e)
    {
        std::cerr << "\nMemory allocation failed: " << e.what();
        delete object;
    }
}
std::vector<Sprite *> GameMap::getNearbySprites(const Sprite *sprite) const
{
    std::vector<Sprite *> nearby;
    sf::FloatRect bounds = sprite->getBounds();

    // Calculate grid cells to check
    int startX = std::max(0, static_cast<int>(bounds.left / GRID_SIZE));
    int startY = std::max(0, static_cast<int>(bounds.top / GRID_SIZE));
    int endX = std::min(gridDimensions.x - 1, static_cast<int>((bounds.left + bounds.width) / GRID_SIZE));
    int endY = std::min(gridDimensions.y - 1, static_cast<int>((bounds.top + bounds.height) / GRID_SIZE));

    // Get sprites from relevant cells
    std::unordered_set<Sprite *> uniqueSprites;
    for (int x = startX; x <= endX; ++x)
    {
        for (int y = startY; y <= endY; ++y)
        {
            for (Sprite *other : spatialGrid[x][y])
            {
                if (other != sprite && uniqueSprites.insert(other).second)
                {
                    nearby.push_back(other);
                }
            }
        }
    }

    return nearby;
}

std::vector<sf::Vector2f> GameMap::getTransformedBounds(const sf::Sprite &sprite) const
{
    // Cache transformed bounds
    const float currentTime = transformClock.getElapsedTime().asSeconds();
    auto *spritePtr = dynamic_cast<const Sprite *>(&sprite);

    if (spritePtr)
    {
        auto &cache = boundsCache[spritePtr];
        if (currentTime - cache.timestamp < 1.0f / 60.0f)
        {
            return cache.vertices;
        }

        const auto &transform = sprite.getTransform();
        const auto &localBounds = sprite.getLocalBounds();

        cache.vertices = {
            transform.transformPoint(0, 0),
            transform.transformPoint(localBounds.width, 0),
            transform.transformPoint(localBounds.width, localBounds.height),
            transform.transformPoint(0, localBounds.height)};
        cache.timestamp = currentTime;
        return cache.vertices;
    }

    // Fallback for non-cached sprites
    const auto &transform = sprite.getTransform();
    const auto &localBounds = sprite.getLocalBounds();
    return {
        transform.transformPoint(0, 0),
        transform.transformPoint(localBounds.width, 0),
        transform.transformPoint(localBounds.width, localBounds.height),
        transform.transformPoint(0, localBounds.height)};
}
float dotProduct(const sf::Vector2f &v1, const sf::Vector2f &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

sf::Vector2f normalize(const sf::Vector2f &v)
{
    float length = std::sqrt(v.x * v.x + v.y * v.y);
    if (length != 0)
    {
        return sf::Vector2f(v.x / length, v.y / length);
    }
    return v; // Return zero vector if length is zero
}

bool GameMap::checkSATCollision(const std::vector<sf::Vector2f> &vertices1, const std::vector<sf::Vector2f> &vertices2)
{
    for (size_t i = 0; i < vertices1.size(); ++i)
    {
        // Calculate the edge and its perpendicular axis
        sf::Vector2f edge = vertices1[(i + 1) % vertices1.size()] - vertices1[i];
        sf::Vector2f axis(-edge.y, edge.x);
        axis = normalize(axis);

        float min1 = std::numeric_limits<float>::max();
        float max1 = std::numeric_limits<float>::lowest();
        for (const auto &vertex : vertices1)
        {
            float projection = dotProduct(vertex, axis);
            min1 = std::min(min1, projection);
            max1 = std::max(max1, projection);
        }

        float min2 = std::numeric_limits<float>::max();
        float max2 = std::numeric_limits<float>::lowest();
        for (const auto &vertex : vertices2)
        {
            float projection = dotProduct(vertex, axis);
            min2 = std::min(min2, projection);
            max2 = std::max(max2, projection);
        }

        // Check for non-overlapping intervals
        if (max1 < min2 || max2 < min1)
        {
            return false; // Separation found, no collision
        }
    }

    return true; // No separation found, collision detected
}

bool GameMap::checkCollision(const sf::Sprite &sprite1, const sf::Sprite &sprite2)
{
    const auto &bounds1 = getTransformedBounds(sprite1);
    const auto &bounds2 = getTransformedBounds(sprite2);

    // Check collision using the SAT algorithm on both sets of vertices
    return checkSATCollision(bounds1, bounds2) && checkSATCollision(bounds2, bounds1);
}