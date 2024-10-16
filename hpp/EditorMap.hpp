#pragma once
class Player;
class Entity;
class EntityFactory;
class TextBox;
class Item;

class EditorMap {
public:
    EditorMap(sf::RenderWindow& window);
    EditorMap(const std::string& fname, sf::RenderWindow& window);
    ~EditorMap() = default;

    void saveToFile(const std::string& fname);
    void loadFromFile(const std::string& fname);
    void changePart(int x, int y);
    sf::FloatRect getPartBounds();
    bool handleMenuClick(const sf::Vector2i& mousePosition);
    void updateEntityProperty(int index, const std::string& property, const std::string& value);
    void removeEntity(int index);
    void addEntity(int x, int y, int w, int h, const std::string& type);
    void drawEditorEntities(sf::RenderWindow& window);

    struct PlacedEntity {
        sf::Sprite sprite;
        std::string type;
        std::unordered_map<std::string, std::string> properties;
        std::string texturePath;
        sf::Texture texture;
    };

    class Menu {
    public:
        Menu(const std::vector<std::string>& entityPaths, const std::vector<std::string>& texturePaths, sf::RenderWindow& window);
        void draw();
        void selectNext();
        void selectPrevious();
        const std::string& getSelectedName() const;
        bool isOpen = false;
        int selectedIndex = 0;
        bool isEntitySelected() const { return selectedIndex < entityTextures.size(); }

        std::vector<sf::Texture> entityTextures;
        std::vector<std::string> entityNames;
        std::vector<sf::Texture> textures;
        static std::vector<std::string> textureNames;

    private:
        sf::RenderWindow& window;
        std::string getFileNameWithoutExtension(const std::string& path);
    };

    class PropertyEditor {
    public:
        void setup(sf::Font& font);
        void updateForEntity(PlacedEntity* entity);
        void draw(sf::RenderWindow& window);
        void handleInput(sf::Event& event, sf::RenderWindow& window);
        void applyChanges();
        bool isOpen = false;
        PlacedEntity* selectedEntity = nullptr;
        void handleTextInput(char inputChar);

    private:
        void wrapText(sf::Text& text, float maxWidth);
        float calculateRequiredHeight(const sf::Text& text, float maxWidth);
        void adjustLayout();
        void adjustBackgroundSize();

        sf::RectangleShape background;
        std::vector<sf::Text> labels;
        std::vector<sf::RectangleShape> inputBoxes;
        std::vector<sf::Text> inputTexts;
        int selectedInputBox = -1;
        sf::Font* font;
    };

    Menu menu;
    std::vector<PlacedEntity> placedEntities;
    PropertyEditor propertyEditor;

    static std::unordered_map<std::string, sf::Texture> entityTextures;

private:
    int mx = 0, my = 0;
    sf::View view;
    sf::RenderWindow& window;
};