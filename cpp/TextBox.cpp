#include "../hpp/libs.hpp"

TextBox::TextBox(sf::Vector2f pos, const std::string& text, float letterDelay, const std::string& imgPath)
    : Sprite(sf::Vector2f(pos)),
      letterDelay(letterDelay),
      elapsedTime(0.0f),
      currentLetter(0),
      visible(false),
      hasCharacterImage(false),
      highlighted(false),
      m_originalResolution(1024.0f, 768.0f),
      m_baseTextSize(18.0f)
{
    // Initialize visual elements
    background.setFillColor(sf::Color(0, 0, 0, 200));
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2.0f);
    
    // Load font
    if (!font.loadFromFile("../fonts/ARIAL.TTF")) {
        std::cerr << "Failed to load font" << std::endl;
    }
    
    // Initialize text
    visibleContent.setFont(font);
    visibleContent.setString(text);
    visibleContent.setFillColor(sf::Color::White);
    visibleContent.setCharacterSize(m_baseTextSize);
    
    // Set character image
    setCharacterImage(imgPath);
    
    // Load sound
    if (!completeBuffer.loadFromFile("../audio/saveSpawnPoint.wav")) {
        std::cerr << "Failed to load text complete sound" << std::endl;
    }
    completeSound.setBuffer(completeBuffer);
    
    // Set layer priority
    priorityLayer = 100;
}

std::string TextBox::processEscapeSequences(const std::string& input) {
    std::string result;
    result.reserve(input.length()); // Preallocate for efficiency
    
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '\\' && i + 1 < input.length()) {
            switch (input[i + 1]) {
                case 'n': result += '\n'; ++i; break;
                case 't': result += '\t'; ++i; break;
                case '\\': result += '\\'; ++i; break;
                default: result += input[i];
            }
        } else {
            result += input[i];
        }
    }
    return result;
}

void TextBox::setString(const std::string& text)
{
    if (!text.empty()) {
        textContent = processEscapeSequences(text);
        visibleContent.setString("");
        currentLetter = 0;
        elapsedTime = 0.0f;
        visible = true;
        highlighted = false;
        visibleContent.setFillColor(sf::Color::White);
    } else {
        visible = false;
    }
}

void TextBox::update(float deltaTime, const sf::Vector2u& screenres)
{
    if (visible && !highlighted && currentLetter < textContent.size())
    {
        elapsedTime += deltaTime;
        if (elapsedTime >= letterDelay)
        {
            elapsedTime -= letterDelay;
            std::string currentText = visibleContent.getString();
            currentText += textContent[currentLetter];
            visibleContent.setString(currentText);
            currentLetter++;
        }
    }
}

void TextBox::updateTextBoxPosition(const sf::RenderWindow& window) const
{
    sf::Vector2f viewSize = window.getView().getSize();
    sf::Vector2f viewCenter = window.getView().getCenter();
    
    // Calculate uniform scale factor
    float scaleFactor = std::min(viewSize.x / m_originalResolution.x, 
                                viewSize.y / m_originalResolution.y);
    
    // Calculate base dimensions with uniform scaling
    float imageWidth = hasCharacterImage ? 70.0f * scaleFactor : 0.0f;
    float textBoxWidth = (viewSize.x / 3.0f) * scaleFactor;
    
    // Count lines for height calculation
    std::string currentText = visibleContent.getString();
    int lineCount = 1;
    for (char c : currentText) {
        if (c == '\n') lineCount++;
    }
    float lineHeight = 25.0f * scaleFactor;
    float textBoxHeight = std::max(70.0f * scaleFactor, 
                                 static_cast<float>(lineCount) * lineHeight);
    
    // Calculate centered position
    float xPos = viewCenter.x - ((textBoxWidth + imageWidth) / 2.0f);
    // Position the textbox lower by placing it at 80% of the screen height instead of at the previous position
    float yPos = viewCenter.y + (viewSize.y * 0.35f) - (textBoxHeight / 2.0f); 
    
    // Update positions with proper scaling
    background.setPosition(xPos + imageWidth, yPos);
    visibleContent.setPosition(xPos + imageWidth + (10.0f * scaleFactor), 
                             yPos + (10.0f * scaleFactor));
    
    if (hasCharacterImage) {
        characterSprite.setPosition(xPos, yPos);
    }
    
    // Update sizes
    background.setSize(sf::Vector2f(textBoxWidth, textBoxHeight));
}

void TextBox::updateTextBoxScale(const sf::RenderWindow& window) const
{
    sf::Vector2f viewSize = window.getView().getSize();
    
    // Use a single scale factor to maintain aspect ratio
    float scaleFactor = std::min(viewSize.x / m_originalResolution.x, 
                                viewSize.y / m_originalResolution.y);
    
    // Scale text size uniformly
    visibleContent.setCharacterSize(static_cast<unsigned int>(m_baseTextSize * scaleFactor));
    
    if (hasCharacterImage) {
        // Use the same scale for both dimensions to maintain aspect ratio
        float targetSize = 70.0f * scaleFactor;
        float scale = targetSize / characterSprite.getLocalBounds().height;
        characterSprite.setScale(scale, scale);
    }
}

void TextBox::draw(sf::RenderWindow& window) const
{
    if (visible)
    {
        // Store current view
        sf::View originalView = window.getView();
        
        // Create UI view that maintains aspect ratio
        sf::View uiView = window.getDefaultView();
        float windowRatio = window.getSize().x / static_cast<float>(window.getSize().y);
        float targetRatio = m_originalResolution.x / m_originalResolution.y;
        
        if (windowRatio > targetRatio) {
            // Window is wider, scale by height
            float viewHeight = m_originalResolution.y;
            float viewWidth = viewHeight * windowRatio;
            uiView.setSize(viewWidth, viewHeight);
        } else {
            // Window is taller, scale by width
            float viewWidth = m_originalResolution.x;
            float viewHeight = viewWidth / windowRatio;
            uiView.setSize(viewWidth, viewHeight);
        }
        
        uiView.setCenter(uiView.getSize().x / 2.0f, uiView.getSize().y / 2.0f);
        window.setView(uiView);
        
        // Update position and scale
        updateTextBoxScale(window);
        updateTextBoxPosition(window);
        
        // Draw elements
        if (hasCharacterImage)
        {
            window.draw(characterSprite);
        }
        
        window.draw(background);
        window.draw(visibleContent);
        
        // Restore original view
        window.setView(originalView);
    }
}

void TextBox::showFullText()
{
    if (visible) {
        visibleContent.setString(textContent);
        currentLetter = textContent.size();
    }
}

void TextBox::setHighlighted(bool highlight)
{
    highlighted = highlight;
    visibleContent.setFillColor(highlight ? sf::Color::Red : sf::Color::White);
}

void TextBox::playSound()
{
    completeSound.play();
}

void TextBox::resetState()
{
    highlighted = false;
    visibleContent.setFillColor(sf::Color::White);
}

void TextBox::setCharacterImage(const std::string& imagePath)
{
    if (characterTexture.loadFromFile(imagePath))
    {
        characterSprite.setTexture(characterTexture);
        float scaley = 70.0f / characterSprite.getLocalBounds().height;
        float scalex = 70.0f / characterSprite.getLocalBounds().width;
        characterSprite.setScale(scalex, scaley);
        hasCharacterImage = true;
    }
    else
    {
        hasCharacterImage = false;
    }
}