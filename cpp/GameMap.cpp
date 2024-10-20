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

void GameMap::resetObjects()
{
    changePart(0, 0);
    collisionObjects.clear();
    allObjects.clear();
    spawnObjects();
}

void GameMap::spawnObjects()
{
    for (const auto &placedObject : originalObjects)
    {
        sf::Vector2f objectPos = placedObject.sprite.getPosition();
        sf::Vector2f objectSize = placedObject.sprite.getGlobalBounds().getSize();
        Object *newOnject = nullptr;

        if (placedObject.type == "Background")
        {
            // Create a Background object
            newOnject = new Background(
                objectPos.x,
                objectPos.y,
                objectSize.x,
                objectSize.y,
                placedObject.texturePath);
        }
        else if (placedObject.type == "Terrain")
        {
            newOnject = new Terrain(
                static_cast<int>(objectPos.x),
                static_cast<int>(objectPos.y),
                static_cast<int>(objectSize.x),
                static_cast<int>(objectSize.y),
                placedObject.texturePath);
        }
        else
        {
            sf::Transformable transform;
            transform.setPosition(placedObject.sprite.getPosition());
            transform.setScale(placedObject.sprite.getScale());
            newOnject = ObjectFactory::createObject(placedObject.type, transform);
        }

        if (newOnject)
        {
            newOnject->setPosition(objectPos);
            // Set object properties
            auto descriptors = ObjectFactory::getPropertyDescriptors(placedObject.type);
            for (const auto &desc : descriptors)
            {
                auto it = placedObject.properties.find(desc.name);
                if (it != placedObject.properties.end() && desc.setter)
                {
                    desc.setter(newOnject, it->second);
                }
            }
            spawn(newOnject);
        }
    }
}

void GameMap::updateObjects(float deltaTime, const sf::Vector2u &windowSize)
{
    for (const auto &object : allObjects)
    {
        object->update(deltaTime, windowSize);
        object->setPosition(object->getPosition());
    }

    for (size_t i = 0; i < collisionObjects.size(); ++i)
    {
        Object *en1 = dynamic_cast<Object *>(collisionObjects[i]);
        for (size_t j = i + 1; j < collisionObjects.size(); ++j)
        {
            Object *en2 = dynamic_cast<Object *>(collisionObjects[j]);
            if (checkCollision(en1->getSprite(), en2->getSprite()))
            {
                collisionObjects[i]->onCollision(en2);
                collisionObjects[j]->onCollision(en1);
            }
        }
        en1->setPosition(en1->getPosition());
    }
}
void GameMap::removeDeadObjects()
{
    // Remove dead objects from allObjects
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

    // Remove corresponding entries from collisionObject
    collisionObjects.erase(
        std::remove_if(collisionObjects.begin(), collisionObjects.end(),
                       [this](CollisionDetector *collisionDetector)
                       {
                           if (!collisionDetector)
                               return true; // Remove null pointers

                           // Check if the corresponding object exists in allObjects
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
    for (const auto &object : allObjects)
    {
        if (object->isOnScreen())
        {
            object->draw(window);
        }
    }
}

void GameMap::spawn(const std::string &objectName, float x, float y, float rotation)
{
    sf::Transformable transform;
    transform.setPosition(x, y);
    transform.setRotation(rotation);
    Object *object = ObjectFactory::createObject(objectName, transform);
    if (object)
    {
        spawn(object);
    }
}

void GameMap::spawn(Object *object)
{
    if (object)
    {
        try
        {
            auto result = allObjects.emplace(std::unique_ptr<Object>(object));

            CollisionDetector *collider = dynamic_cast<CollisionDetector *>(object);
            if (collider)
            {
                collisionObjects.push_back(collider);
            }
        }
        catch (const std::bad_alloc &e)
        {
            std::cerr << "\nMemory allocation failed: " << e.what();
            delete object;
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