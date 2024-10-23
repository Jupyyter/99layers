#pragma once
class TextBox : public Object {
public:
    TextBox(sf::Vector2f pos,const std::string& text, float letterDelay = 0.05f,const std::string& imgPath="../imgs/hammer.jpg");
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override { return background.getGlobalBounds(); }
    const sf::Sprite getSprite() const override { return characterSprite; }
    bool isOnScreen() const override { return visible; }
    
    void setString(const std::string& text);
    std::string getText() const { return textContent; }
    void setCharacterImage(const std::string& imagePath);

private:
    const Object* parent;  // Reference to parent object (NPC)
    mutable sf::RectangleShape background;  // Made mutable
    std::string textContent;
    mutable sf::Text visibleContent;        // Made mutable
    sf::Font font;
    float letterDelay;
    float elapsedTime;
    int currentLetter;
    bool visible;

    sf::Texture characterTexture;
    mutable sf::Sprite characterSprite;     // Made mutable
    bool hasCharacterImage;
};