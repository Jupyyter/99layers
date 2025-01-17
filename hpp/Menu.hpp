#pragma once

class Menu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::RectangleShape background;
    sf::Text titleText;      // Added title text
    sf::Text settingText;
    sf::RectangleShape musicButton;
    sf::Text musicButtonText;
    
    sf::Text volumeText;
    sf::RectangleShape sliderTrack;
    sf::CircleShape sliderHandle;
    sf::Text volumePercentage;
    bool isDraggingSlider;
    float currentVolume;
    
    bool isVisible;
    std::vector<std::string> musicFiles;
    size_t currentMusicIndex;
    sf::Music& gameplayMusic;
    

public:
    Menu(sf::RenderWindow& window, sf::Music& gameplayMusic);
    void toggleVisibility();
    void draw();
    void handleEvent(const sf::Event& event);
    bool isMenuVisible() const { return isVisible; }
    void updateButtonPosition();
    void updateVolumeFromSlider();
};