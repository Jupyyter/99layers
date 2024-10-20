#pragma once
class Player;
class Object;
class TextBox;
class Item;

class EditorMap {
public:
    EditorMap(sf::RenderWindow& window);
    EditorMap(const std::string& fname, sf::RenderWindow& window);

    void saveToFile(const std::string& fname);
    void loadFromFile(const std::string& fname);
    bool handleMenuClick(const sf::Vector2i& mousePosition);
    void updateObjectProperty(int index, const std::string& property, const std::string& value);
    void removeObject(int index);
    void addObject(int x, int y, int w, int h, const std::string& type);
    void drawEditorObjects(sf::RenderWindow& window);

    struct PlacedObject {
        sf::Sprite sprite;
        std::string type;
        std::unordered_map<std::string, std::string> properties;
        std::string texturePath;
        sf::Texture texture;
    };

    class Menu {
    public:
        Menu(const std::vector<std::string>& objectPaths, const std::vector<std::string>& texturePaths, 
             const std::vector<std::string>& backgroundPaths, sf::RenderWindow& window);
        void draw();
        const std::string& getSelectedName() const;
        bool isBackgroundSelected() const;
        bool isObjectSelected() const;

        bool isOpen = false;
        int selectedIndex = 0;

        sf::RenderWindow& window;
        static std::vector<std::string> textureNames;
        std::vector<sf::Texture> objectTextures;
        std::vector<std::string> objectNames;
        std::vector<sf::Texture> textures;
        std::vector<sf::Texture> backgroundTextures;
        std::vector<std::string> backgroundNames;
        std::string getFileNameWithoutExtension(const std::string& path);

    };

    class PropertyEditor {
    public:
        void setup(sf::Font& font);
        void updateForObject(PlacedObject* object);
        void draw(sf::RenderWindow& window);
        void handleInput(sf::Event& event, sf::RenderWindow& window);
        void applyChanges();

        bool isOpen = false;
        PlacedObject* selectedObject = nullptr;

    private:
        void handleTextInput(char inputChar);
        void wrapText(sf::Text& text, float maxWidth);
        float calculateRequiredHeight(const sf::Text& text, float maxWidth);
        void adjustLayout();

        sf::RectangleShape background;
        std::vector<sf::Text> labels;
        std::vector<sf::RectangleShape> inputBoxes;
        std::vector<sf::Text> inputTexts;
        int selectedInputBox = -1;
        sf::Font* font;
    };

    Menu menu;
    std::vector<PlacedObject> placedObjects;
    PropertyEditor propertyEditor;

    static std::unordered_map<std::string, sf::Texture> objectTextures;

private:
    int mx = 0, my = 0;
    sf::View view;
    sf::RenderWindow& window;
};