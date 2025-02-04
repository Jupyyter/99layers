#pragma once
class TextBox : public Sprite {
public:
    TextBox(sf::Vector2f pos, const std::string& text, float letterDelay = 0.05f, const std::string& imgPath="../imgs/hammer.jpg");
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const override;

    bool isOnScreen() const override { return visible; }
    
    // Core functionality methods
    void setString(const std::string& text);
    std::string getText() const { return textContent; }
    void setCharacterImage(const std::string& imagePath);
    bool isFullyDisplayed() const { return currentLetter >= textContent.size(); }
    void showFullText();
    void setHighlighted(bool highlight);
    void playSound();
    bool isVisible() const { return visible; }
    bool isHighlighted() const { return highlighted; }
    void resetState();

private:
    // UI Elements
    const Object* parent;
    mutable sf::RectangleShape background;
    std::string textContent;
    mutable sf::Text visibleContent;
    sf::Font font;
    
    // Character image elements
    sf::Texture characterTexture;
    mutable sf::Sprite characterSprite;
    bool hasCharacterImage;
    
    // Audio elements
    sf::SoundBuffer completeBuffer;
    mutable sf::Sound completeSound;
    
    // State variables
    float letterDelay;
    float elapsedTime;
    int currentLetter;
    bool visible;
    bool highlighted;
    
    // Scaling and positioning
    sf::Vector2f m_originalResolution;
    float m_baseTextSize;
    
    // Helper methods
    void updateTextBoxPosition(const sf::RenderWindow& window) const;
    void updateTextBoxScale(const sf::RenderWindow& window) const;
    static std::string processEscapeSequences(const std::string& input);
};