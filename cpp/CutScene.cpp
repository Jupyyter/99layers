#include "../hpp/libs.hpp"
#include <algorithm>

CutScene::CutScene(const std::vector<std::string>& imagePaths, const sf::Vector2u& windowSize)
    : currentFrame(0), fadeTime(0), frameDuration(8.0f), // Total duration for each frame (fade in + stay visible + fade out)
      fadeInDuration(2.0f), stayVisibleDuration(4.0f), fadeOutDuration(2.0f), // Durations for each phase
      isFinished(false), isPaused(false), cutSceneFinished(false), windowSize(windowSize) {
    loadTextures(imagePaths);
    if (!textures.empty()) {
        updateSpriteForCurrentFrame();
        currentSprite.setColor(sf::Color(255, 255, 255, 0)); // Start fully transparent
    }
}

void CutScene::loadTextures(const std::vector<std::string>& imagePaths) {
    for (const auto& path : imagePaths) {
        sf::Texture texture;
        if (texture.loadFromFile(path)) {
            textures.push_back(texture);
        } else {
            std::cerr << "Failed to load cutscene image: " << path << std::endl;
        }
    }
}

void CutScene::updateSpriteForCurrentFrame() {
    if (currentFrame < textures.size()) {
        currentSprite.setTexture(textures[currentFrame], true);  // true resets the texture rect
        
        sf::Vector2u textureSize = textures[currentFrame].getSize();
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        float scale = std::min(scaleX, scaleY);

        currentSprite.setScale(scale, scale);

        sf::FloatRect bounds = currentSprite.getLocalBounds();
        currentSprite.setOrigin(bounds.width / 2, bounds.height / 2);
        currentSprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    }
}
void CutScene::handleResize(const sf::Vector2u& newWindowSize) {
    // Update the stored window size
    windowSize = newWindowSize;
    
    // Recalculate the scale and position for the current image
    if (currentFrame < textures.size()) {
        sf::Vector2u textureSize = textures[currentFrame].getSize();
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        float scale = std::min(scaleX, scaleY);

        currentSprite.setScale(scale, scale);

        sf::FloatRect bounds = currentSprite.getLocalBounds();
        currentSprite.setOrigin(bounds.width / 2, bounds.height / 2);
        currentSprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    }
}
bool CutScene::update(float deltaTime) {
    if (isFinished) return true;

    fadeTime += deltaTime;

    if (fadeTime < fadeInDuration) {
        // Fading in
        float alpha = (fadeTime / fadeInDuration) * 255;
        currentSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
    } else if (fadeTime < fadeInDuration + stayVisibleDuration) {
        // Fully visible - no changes needed
        currentSprite.setColor(sf::Color(255, 255, 255, 255)); // Ensure fully opaque
    } else if (fadeTime < frameDuration) {
        // Fading out
        float alpha = (1 - (fadeTime - (fadeInDuration + stayVisibleDuration)) / fadeOutDuration) * 255;
        currentSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
    } else {
        // Frame finished, move to the next frame
        fadeTime = 0; // Reset fadeTime for the next frame
        currentFrame++;
        if (currentFrame >= textures.size()) {
            isFinished = true;
            return true;
        }
        updateSpriteForCurrentFrame();
        currentSprite.setColor(sf::Color(255, 255, 255, 0)); // Start fully transparent for the next frame
    }

    return false;
}

void CutScene::draw(sf::RenderWindow& window) {
    window.draw(currentSprite);
}