#include "../hpp/libs.hpp"
class EditorMap;

GameMap::GameMap(sf::RenderWindow &wndref, bool &gameover)
    : mx(0), my(0), np(1), wndref(wndref), gameOver(&gameover)
{
    view.setSize(wndref.getSize().x, wndref.getSize().y);
}

GameMap::GameMap(std::string fname, sf::RenderWindow &wndref, bool &gameover)
    : mx(0), my(0), np(1), wndref(wndref), gameOver(&gameover)
{
    view.setSize(wndref.getSize().x, wndref.getSize().y);
    loadFromFile(fname);
    collisionEntities.clear();
    allEntities.clear();
}

void GameMap::loadFromFile(const std::string &fname)
{
    std::ifstream file(fname, std::ios::binary);
    if (!file)
    {
        std::cerr << "\nFailed to open file for reading: " << fname;
        return;
    }

    // Clear existing entities
    originalEntities.clear();

    int entityCount;
    file.read(reinterpret_cast<char *>(&entityCount), sizeof(int));

    for (int i = 0; i < entityCount; ++i)
    {
        EditorMap::PlacedEntity entity;

        // Read entity type
        int typeLength;
        file.read(reinterpret_cast<char *>(&typeLength), sizeof(int));
        entity.type.resize(typeLength);
        file.read(&entity.type[0], typeLength);

        // Read position and size
        float x, y, width, height;
        file.read(reinterpret_cast<char *>(&x), sizeof(float));
        file.read(reinterpret_cast<char *>(&y), sizeof(float));
        file.read(reinterpret_cast<char *>(&width), sizeof(float));
        file.read(reinterpret_cast<char *>(&height), sizeof(float));

        // Read texture path
        int texturePathLength;
        file.read(reinterpret_cast<char *>(&texturePathLength), sizeof(int));
        entity.texturePath.resize(texturePathLength);
        file.read(&entity.texturePath[0], texturePathLength);

        // Set up the sprite
        entity.sprite.setPosition(x, y);
        sf::Texture texture;
        std::string fullTexturePath = entity.texturePath;
        if (texture.loadFromFile(fullTexturePath))
        {
            entity.sprite.setTexture(texture);
            if (entity.type == "Terrain")
            {
                entity.sprite.setTextureRect(sf::IntRect(0, 0, width, height));
            }
            else
            {
                float scaleX = width / texture.getSize().x;
                float scaleY = height / texture.getSize().y;
                entity.sprite.setScale(scaleX, scaleY);
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
            entity.properties[name] = value;
        }

        originalEntities.push_back(std::move(entity));
    }
}

void GameMap::changePart(int x, int y)
{
    mx += x;
    my += y;
    np++;
    int wx = wndref.getSize().x, wy = wndref.getSize().y;
    view.setCenter(wx / 2 + wx * mx, wy / 2 + wy * my);
    wndref.setView(view);
}

sf::FloatRect GameMap::getPartBounds() const
{
    int wx = wndref.getSize().x, wy = wndref.getSize().y;
    return sf::FloatRect(mx * wx, my * wy, wx, wy);
}

void GameMap::resetEntities()
{
    changePart(0, 0);
    collisionEntities.clear();
    allEntities.clear();
    spawnEntities();
}

#include "../hpp/libs.hpp"
class EditorMap;

// ... (previous code remains unchanged)

void GameMap::spawnEntities()
{
    for (const auto &placedEntity : originalEntities)
    {
        sf::Vector2f entityPos = placedEntity.sprite.getPosition();
        sf::Vector2f entitySize = placedEntity.sprite.getGlobalBounds().getSize();
        Entity *newEntity = nullptr;

        if (placedEntity.type == "Background")
        {
            // Create a Background entity
            newEntity = new Background(
                entityPos.x,
                entityPos.y,
                entitySize.x,
                entitySize.y,
                placedEntity.texturePath);
        }
        else if (placedEntity.type == "Terrain")
        {
            newEntity = new Terrain(
                static_cast<int>(entityPos.x),
                static_cast<int>(entityPos.y),
                static_cast<int>(entitySize.x),
                static_cast<int>(entitySize.y),
                placedEntity.texturePath);
        }
        else
        {
            sf::Transformable transform;
            transform.setPosition(placedEntity.sprite.getPosition());
            transform.setScale(placedEntity.sprite.getScale());
            newEntity = EntityFactory::createEntity(placedEntity.type, transform);
        }

        if (newEntity)
        {
            newEntity->setPosition(entityPos);
            // Set entity properties
            auto descriptors = EntityFactory::getPropertyDescriptors(placedEntity.type);
            for (const auto &desc : descriptors)
            {
                auto it = placedEntity.properties.find(desc.name);
                if (it != placedEntity.properties.end() && desc.setter)
                {
                    desc.setter(newEntity, it->second);
                }
            }
            spawn(newEntity);
        }
    }
}

void GameMap::updateEntities(float deltaTime, const sf::Vector2u &windowSize)
{
    for (const auto &entity : allEntities)
    {
        entity->update(deltaTime, windowSize);
    }

    for (size_t i = 0; i < collisionEntities.size(); ++i)
    {
        Entity *en1 = dynamic_cast<Entity *>(collisionEntities[i]);
        for (size_t j = i + 1; j < collisionEntities.size(); ++j)
        {
            Entity *en2 = dynamic_cast<Entity *>(collisionEntities[j]);
            if (checkCollision(en1->getSprite(), en2->getSprite()))
            {
                collisionEntities[i]->onCollision(en2);
                collisionEntities[j]->onCollision(en1);
            }
        }
    }
}
void GameMap::removeDeadEntities()
{
    // Remove dead entities from allEntities
    for (auto it = allEntities.begin(); it != allEntities.end();)
    {
        if ((*it)->shouldBeDead)
        {
            it = allEntities.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Remove corresponding entries from collisionEntities
    collisionEntities.erase(
        std::remove_if(collisionEntities.begin(), collisionEntities.end(),
                       [this](CollisionDetector *collisionDetector)
                       {
                           if (!collisionDetector)
                               return true; // Remove null pointers

                           // Check if the corresponding Entity exists in allEntities
                           return std::none_of(allEntities.begin(), allEntities.end(),
                                               [collisionDetector](const std::unique_ptr<Entity> &entity)
                                               {
                                                   return entity.get() == dynamic_cast<Entity *>(collisionDetector);
                                               });
                       }),
        collisionEntities.end());
}
void GameMap::drawEntities(sf::RenderWindow &window) const
{
    for (const auto &entity : allEntities)
    {
        if (entity->isOnScreen())
        {
            entity->draw(window);
        }
    }
}

void GameMap::spawn(const std::string &entityName, float x, float y, float rotation)
{
    sf::Transformable transform;
    transform.setPosition(x, y);
    transform.setRotation(rotation);
    Entity *entity = EntityFactory::createEntity(entityName, transform);
    if (entity)
    {
        spawn(entity);
    }
}

void GameMap::spawn(Entity *entity)
{
    if (entity)
    {
        try
        {
            auto result = allEntities.emplace(std::unique_ptr<Entity>(entity));

            CollisionDetector *collider = dynamic_cast<CollisionDetector *>(entity);
            if (collider)
            {
                collisionEntities.push_back(collider);
            }
        }
        catch (const std::bad_alloc &e)
        {
            std::cerr << "\nMemory allocation failed: " << e.what();
            delete entity;
        }
    }
}
bool GameMap::checkCollision(const sf::Sprite &sprite1, const sf::Sprite &sprite2)
{
    const auto &bounds1 = getTransformedBounds(sprite1);
    const auto &bounds2 = getTransformedBounds(sprite2);

    return checkSATCollision(bounds1, bounds2) && checkSATCollision(bounds2, bounds1);
}
std::vector<sf::Vector2f> GameMap::getTransformedBounds(const sf::Sprite &sprite)
{
    const auto &transform = sprite.getTransform();
    const auto &localBounds = sprite.getLocalBounds();

    std::vector<sf::Vector2f> vertices = {
        transform.transformPoint(0, 0),
        transform.transformPoint(localBounds.width, 0),
        transform.transformPoint(localBounds.width, localBounds.height),
        transform.transformPoint(0, localBounds.height)};

    return vertices;
}
bool GameMap::checkSATCollision(const std::vector<sf::Vector2f> &vertices1, const std::vector<sf::Vector2f> &vertices2)
{
    for (size_t i = 0; i < vertices1.size(); ++i)
    {
        sf::Vector2f edge = vertices1[(i + 1) % vertices1.size()] - vertices1[i];
        sf::Vector2f axis(-edge.y, edge.x);
        float axisLengthSquared = axis.x * axis.x + axis.y * axis.y;

        float min1 = std::numeric_limits<float>::max();
        float max1 = std::numeric_limits<float>::lowest();
        for (const auto &vertex : vertices1)
        {
            float projection = (vertex.x * axis.x + vertex.y * axis.y) / axisLengthSquared;
            min1 = std::min(min1, projection);
            max1 = std::max(max1, projection);
        }

        float min2 = std::numeric_limits<float>::max();
        float max2 = std::numeric_limits<float>::lowest();
        for (const auto &vertex : vertices2)
        {
            float projection = (vertex.x * axis.x + vertex.y * axis.y) / axisLengthSquared;
            min2 = std::min(min2, projection);
            max2 = std::max(max2, projection);
        }

        if (max1 < min2 || max2 < min1)
        {
            return false;
        }
    }

    return true;
}