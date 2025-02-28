#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class CutScene {
public:
    CutScene(const std::vector<std::string>& imagePaths, const sf::Vector2u& windowSize);
    bool update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool cutSceneFinished;

private:
    std::vector<sf::Texture> textures;
    sf::Sprite currentSprite;
    size_t currentFrame;
    float fadeTime;
    float frameDuration;       // Total duration for each frame (fade in + stay visible + fade out)
    float fadeInDuration;      // Duration of the fade-in phase
    float stayVisibleDuration; // Duration the image stays fully visible
    float fadeOutDuration;    // Duration of the fade-out phase
    bool isFinished;
    bool isPaused;
    sf::Vector2u windowSize;

    void loadTextures(const std::vector<std::string>& imagePaths);
    void updateSpriteForCurrentFrame();
};