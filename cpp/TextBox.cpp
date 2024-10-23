#include "../hpp/libs.hpp"

TextBox::TextBox(sf::Vector2f pos,const std::string& text, float letterDelay,const std::string& imgPath)
    : Object(sf::Vector2f(pos)), // Position will be updated based on parent
      letterDelay(letterDelay),
      elapsedTime(0.0f),
      currentLetter(0),
      visible(false),
      hasCharacterImage(false)
{
    background.setFillColor(sf::Color(255, 255, 255, 200));
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(2.0f);
    font.loadFromFile("../fonts/ARIAL.TTF");
    setCharacterImage(imgPath);
    visibleContent.setFont(font);
    visibleContent.setString(text);
    visibleContent.setFillColor(sf::Color::Black);
    visibleContent.setCharacterSize(18);
    priorityLayer = 100; // Ensure text box appears above other objects
}

void TextBox::setString(const std::string& text)
{
    if(text.length() != 0) {
        textContent = text;
        visibleContent.setString("");
        currentLetter = 0;
        elapsedTime = 0.0f;
        visible = true;
    }
    else {
        visible = false;
    }
}

void TextBox::update(float deltaTime, const sf::Vector2u& screenres)
{
    if (visible)
    {


        elapsedTime += deltaTime;
        if (elapsedTime >= letterDelay)
        {
            elapsedTime = 0.0f;
            if (currentLetter < textContent.size())
            {
                visibleContent.setString(visibleContent.getString() + textContent[currentLetter]);
                currentLetter++;
            }
        }
    }
}

void TextBox::draw(sf::RenderWindow& window) const
{
    if (visible)
    {
        sf::View originalView = window.getView();
        window.setView(window.getDefaultView());

        float imageWidth = hasCharacterImage ? 70.0f : 0.0f;
        float textBoxWidth = window.getSize().x / 3;

        // Use position directly since it's already in screen coordinates
        sf::Vector2f basePos = position;
        
        background.setPosition(basePos + sf::Vector2f(imageWidth, 0));
        visibleContent.setPosition(basePos + sf::Vector2f(imageWidth + 10.0f, 10.0f));
        background.setSize(sf::Vector2f(textBoxWidth, 70.0f));

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