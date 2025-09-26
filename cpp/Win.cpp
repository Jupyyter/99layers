#include "../hpp/libs.hpp"

Win::Win(sf::RenderWindow& window) : window(window), isButtonHovered(false)
{
    // Load background image
    if (!backgroundTexture.loadFromFile("../imgs/endi.png"))
    {
        std::cout << "Failed to load win background image, using fallback color" << std::endl;
    }
    
    // Initialize background sprite
    backgroundSprite.setTexture(backgroundTexture);
    
    // Set up the button
    button.setSize(sf::Vector2f(400, 50));
    button.setFillColor(sf::Color(100, 100, 100, 200));
    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(2);
    
    // Load font
    if (!font.loadFromFile("../fonts/arial.ttf"))
    {
        std::cout << "Failed to load font, trying backup font" << std::endl;
        if (!font.loadFromFile("../fonts/DejaVuSans.ttf")) 
        {
            std::cout << "Failed to load backup font" << std::endl;
        }
    }
    
    // Set up button text
    buttonText.setFont(font);
    buttonText.setString("wow i dont want to play this game anymore");
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::White);
    
    // Immediately update positioning on construction
    sf::Vector2u windowSize = window.getSize();
    
    // Calculate background scale
    if (backgroundTexture.getSize().x > 0 && backgroundTexture.getSize().y > 0)
    {
        float scaleX = static_cast<float>(windowSize.x) / backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(windowSize.y) / backgroundTexture.getSize().y;
        float scale = std::max(scaleX, scaleY); // Use max to cover the entire window
        
        backgroundSprite.setScale(scale, scale);
        
        // Center the background
        backgroundSprite.setPosition(
            (windowSize.x - backgroundTexture.getSize().x * scale) / 2.0f,
            (windowSize.y - backgroundTexture.getSize().y * scale) / 2.0f
        );
    }
    
    // Update button position to center of window
    button.setOrigin(button.getSize().x / 2.0f, button.getSize().y / 2.0f);
    button.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    
    // Update text position
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f, 
                         textBounds.top + textBounds.height / 2.0f);
    buttonText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
}

void Win::updatePositions()
{
    // Update background sprite to cover the entire window
    sf::Vector2u windowSize = window.getSize();
    
    // Calculate scale to fit the window while maintaining aspect ratio
    if (backgroundTexture.getSize().x > 0 && backgroundTexture.getSize().y > 0)
    {
        float scaleX = static_cast<float>(windowSize.x) / backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(windowSize.y) / backgroundTexture.getSize().y;
        float scale = std::max(scaleX, scaleY); // Use max to cover the entire window
        
        backgroundSprite.setScale(scale, scale);
        
        // Center the background
        backgroundSprite.setPosition(
            (windowSize.x - backgroundTexture.getSize().x * scale) / 2.0f,
            (windowSize.y - backgroundTexture.getSize().y * scale) / 2.0f
        );
    }
    
    // Update button position to center of window
    button.setOrigin(button.getSize().x / 2.0f, button.getSize().y / 2.0f);
    button.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    
    // Update text position
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f, 
                         textBounds.top + textBounds.height / 2.0f);
    buttonText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
}

void Win::draw()
{
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());
    // Draw a colored background as a fallback in case sprite doesn't work
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(50, 200, 50));
    window.draw(background);
    
    // Draw the sprite background if available
    if (backgroundTexture.getSize().x > 0)
    {
        window.draw(backgroundSprite);
    }
    
    // Draw button with outline for visibility
    window.draw(button);
    
    // Draw the text if font loaded, otherwise draw a placeholder
    if (font.getInfo().family != "")
    {
        window.draw(buttonText);
    }
    else
    {
        // Draw placeholder text (just a rectangle) if font didn't load
        sf::RectangleShape textPlaceholder(sf::Vector2f(360, 30));
        textPlaceholder.setFillColor(sf::Color::White);
        textPlaceholder.setOrigin(180, 15);
        textPlaceholder.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
        window.draw(textPlaceholder);
    }
    window.setView(originalView);
}

void Win::updateButtonColor()
{
    // Get mouse position
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    
    // Check if mouse is over the button
    if (button.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
    {
        button.setFillColor(sf::Color(150, 150, 150, 240)); // Lighter gray when hovered
        button.setOutlineColor(sf::Color::Yellow); // Highlight with yellow outline
        isButtonHovered = true;
    }
    else
    {
        button.setFillColor(sf::Color(100, 100, 100, 220)); // Default color
        button.setOutlineColor(sf::Color::White);
        isButtonHovered = false;
    }
}

bool Win::isExitButtonClicked()
{
    // Get mouse position
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    
    // Check if mouse is over the button
    if (button.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
    {
        return true;
    }
    
    return false;
}