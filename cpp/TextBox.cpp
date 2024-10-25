#include "../hpp/libs.hpp"

TextBox::TextBox(sf::Vector2f pos, const std::string& text, float letterDelay, const std::string& imgPath)
    : Sprite(sf::Vector2f(pos)),
      letterDelay(letterDelay),
      elapsedTime(0.0f),
      currentLetter(0),
      visible(false),
      hasCharacterImage(false),
      highlighted(false)
{
    background.setFillColor(sf::Color(0, 0, 0, 200));
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2.0f);
    font.loadFromFile("../fonts/ARIAL.TTF");
    setCharacterImage(imgPath);
    visibleContent.setFont(font);
    visibleContent.setString(text);
    visibleContent.setFillColor(sf::Color::White);
    visibleContent.setCharacterSize(18);
    priorityLayer = 100;

    if (!completeBuffer.loadFromFile("../audio/saveSpawnPoint.wav")) {
        std::cerr << "Failed to load text complete sound" << std::endl;
    }
    completeSound.setBuffer(completeBuffer);
}
std::string processEscapeSequences(const std::string& input) {
    std::string result;
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '\\' && i + 1 < input.length()) {
            switch (input[i + 1]) {
                case 'n':
                    result += '\n';
                    ++i;
                    break;
                case 't':
                    result += '\t';
                    ++i;
                    break;
                case '\\':
                    result += '\\';
                    ++i;
                    break;
                default:
                    result += input[i];
            }
        } else {
            result += input[i];
        }
    }
    return result;
}
void TextBox::setString(const std::string& text)
{
    if(text.length() != 0) {
        // Process the text for escape sequences
        textContent = processEscapeSequences(text);
        visibleContent.setString("");
        currentLetter = 0;
        elapsedTime = 0.0f;
        visible = true;
        highlighted = false;
        visibleContent.setFillColor(sf::Color::White);
    }
    else {
        visible = false;
    }
}

void TextBox::update(float deltaTime, const sf::Vector2u& screenres)
{
    if (visible && !highlighted)
    {
        elapsedTime += deltaTime;
        if (elapsedTime >= letterDelay)
        {
            elapsedTime = 0.0f;
            if (currentLetter < textContent.size())
            {
                std::string currentText = visibleContent.getString();
                currentText += textContent[currentLetter];
                visibleContent.setString(currentText);
                currentLetter++;
            }
        }
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
    visibleContent.setFillColor(highlight ? sf::Color::Red : sf::Color::Black);
}

void TextBox::playSound()
{
    completeSound.play();
}

void TextBox::resetState()
{
    highlighted = false;
    visibleContent.setFillColor(sf::Color::Black);
}

void TextBox::draw(sf::RenderWindow& window) const
{
    if (visible)
    {
        sf::View originalView = window.getView();
        window.setView(window.getDefaultView());

        float imageWidth = hasCharacterImage ? 70.0f : 0.0f;
        float textBoxWidth = window.getSize().x / 3;
        
        // Adjust height based on number of lines
        std::string currentText = visibleContent.getString();
        int lineCount = 1;
        for (char c : currentText) {
            if (c == '\n') lineCount++;
        }
        float textBoxHeight = std::max(70.0f, static_cast<float>(lineCount * 25));

        sf::Vector2f basePos = position;
        
        background.setPosition(basePos + sf::Vector2f(imageWidth, 0));
        visibleContent.setPosition(basePos + sf::Vector2f(imageWidth + 10.0f, 10.0f));
        background.setSize(sf::Vector2f(textBoxWidth, textBoxHeight));

        if (hasCharacterImage)
        {
            characterSprite.setPosition(basePos);
            window.draw(characterSprite);
        }

        window.draw(background);
        window.draw(visibleContent);
        window.setView(originalView);
    }
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