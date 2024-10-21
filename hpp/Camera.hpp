#pragma once
class Camera {
public:
    Camera(sf::RenderWindow& window);

    void setPosition(const sf::Vector2f& position, bool teleport = false);
    void setSize(const sf::Vector2f& size);
    void setZoom(float zoom);
    void setSmoothness(float smoothness);

    void update(float deltaTime);
    void applyTo(sf::RenderWindow& window);

    const sf::Vector2f& getPosition() const { return m_position; }
    const sf::Vector2f& getSize() const { return m_size; }
    float getZoom() const { return m_zoom; }

    void moveToNextPart(int dx, int dy, bool teleport = false);

private:
    sf::View m_view;
    sf::Vector2f m_position;
    sf::Vector2f m_targetPosition;
    sf::Vector2f m_size;
    float m_zoom;
    float m_smoothness;
};