#pragma once
class Player;
class Entity;
class EntityFactory;
class TextBox;
class Item;

class EditorMap
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
    class Menu
    {
    public:
        Menu(const std::vector<std::string> &entityPaths, const std::vector<std::string> &texturePaths, sf::RenderWindow &window);
        void draw();
        void selectNext();
        void selectPrevious();
        bool isOpen = false;
        int selectedIndex;
        bool isEntitySelected() const { return selectedIndex < entityTextures.size(); }

        std::vector<sf::Texture> entityTextures;
        std::vector<std::string> entityNames;
        std::vector<sf::Texture> textures;
        std::vector<std::string> textureNames;

        const std::string &getSelectedName() const;

    private:
        sf::RenderWindow &window;
        std::string getFileNameWithoutExtension(const std::string &path);
    };

    EditorMap(sf::RenderWindow &wndref);
    EditorMap(std::string fname, sf::RenderWindow &wndref);
    EditorMap(const EditorMap &) = delete;
    EditorMap &operator=(const EditorMap &) = delete;
    ~EditorMap();

    void draw();
    void addObject(int x, int y, int w, int h);
    void removeObject(int index);
    void saveToFile(std::string fname);
    void changePart(int x, int y);
    sf::FloatRect getPartBounds();
    std::vector<sf::FloatRect> getObjectBounds();
    std::vector<sf::FloatRect> getEntityBounds();

    bool handleMenuClick(const sf::Vector2i &mousePosition, sf::RenderWindow &window);
    const std::string &getSelectedName() const;
    const sf::Texture *getSelectedTexture() const;
    const sf::Texture *getEntityTexture(const std::string &entityName) const;

    void removeEntity(int x, int y);
    void removeEntity(int index);
    void addEntity(int x, int y, const std::string &entityType);
    void drawEditorEntities(sf::RenderWindow &window, const Entity::PlacedEntity *selectedEntity, bool &isOpen);
    bool *gameOver;               // this should not be here
    std::vector<Item *> allItems; // this shoud also not be here but its kinda justified
    Menu menu;
    std::vector<std::unique_ptr<Entity::PlacedEntity>> placedEntities;

    class PropertyEditor
    {
    public:
        void setup(sf::Font &loadedFont);
        void updateForEntity(Entity::PlacedEntity *entity, sf::Font &font);
        void draw(sf::RenderWindow &window);
        void handleInput(sf::Event &event, sf::RenderWindow &window);
        void applyChanges();
        bool isOpen = false;
        Entity::PlacedEntity *selectedEntity = nullptr;

    private:
        void wrapText(sf::Text &text, float maxWidth);
        float calculateRequiredHeight(const sf::Text &text, float maxWidth);
        void adjustLayout();
        void adjustBackgroundSize();

        sf::RectangleShape background;
        std::vector<sf::Text> labels;
        std::vector<sf::RectangleShape> inputBoxes;
        std::vector<sf::Text> inputTexts;
        int selectedInputBox;
        sf::Font *font;
    };

    PropertyEditor propertyEditor;

private:
    static std::unordered_map<std::string, sf::Texture> entityTextures;
    std::unordered_map<int, std::unordered_map<int, std::vector<Object *>>> obj;
    int mx, my, np;
    sf::View view;
    sf::RenderWindow &wndref;
};