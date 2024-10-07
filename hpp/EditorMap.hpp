#pragma once
class Player;
class Entity;
class EntityFactory;
class TextBox;
class Item;

class EditorMap
{

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
        static std::vector<std::string> textureNames;

        const std::string &getSelectedName() const;

    private:
        sf::RenderWindow &window;
        std::string getFileNameWithoutExtension(const std::string &path);
    };
struct PlacedEntity {
    sf::Sprite sprite;
    std::string type;
    std::unordered_map<std::string, std::string> properties;
    std::string texturePath;
    sf::Texture texture;
};

    EditorMap(sf::RenderWindow &wndref);
    EditorMap(const std::string fname, sf::RenderWindow &wndref);
    EditorMap(const EditorMap &) = delete;
    EditorMap &operator=(const EditorMap &) = delete;
    ~EditorMap();

    void saveToFile(const std::string &fname);
    void changePart(int x, int y);
    sf::FloatRect getPartBounds();

    bool handleMenuClick(const sf::Vector2i &mousePosition, sf::RenderWindow &window);
    const std::string &getSelectedName() const;
    const sf::Texture *getSelectedTexture() const;
    const sf::Texture *getEntityTexture(const std::string &entityName) const;

    void updateEntityProperty(int index, const std::string& property, const std::string& value);
    void removeEntity(int x, int y);
    void removeEntity(int index);
    void addEntity(int x, int y, int w, int h, const std::string &type);
    void loadFromFile(const std::string& fname);
    void drawEditorEntities(sf::RenderWindow &window, const PlacedEntity *selectedEntity, bool &isOpen);
    Menu menu;
    std::vector<PlacedEntity> placedEntities;

    class PropertyEditor
    {
    public:
        void setup(sf::Font &loadedFont);
        void updateForEntity(PlacedEntity *entity, sf::Font &font);
        void draw(sf::RenderWindow &window);
        void handleInput(sf::Event &event, sf::RenderWindow &window);
        void applyChanges();
        bool isOpen = false;
        PlacedEntity *selectedEntity = nullptr;

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

    static std::unordered_map<std::string, sf::Texture> entityTextures;
private:
    int mx, my, np;
    sf::View view;
    sf::RenderWindow &wndref;
};