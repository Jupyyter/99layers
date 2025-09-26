#include "../hpp/libs.hpp"

Camera::Camera(sf::RenderWindow& window)
    : m_position(window.getSize().x / 2.f, window.getSize().y / 2.f)
    , m_targetPosition(m_position)
    , m_size(window.getSize())
    , m_zoom(1.f)
    , m_smoothness(5.f)
{
    m_view.setSize(m_size);
    m_view.setCenter(m_position);
}

void Camera::setPosition(const sf::Vector2f& position, bool teleport) {
    if (teleport) {
        m_position = position;
        m_targetPosition = position;
        m_view.setCenter(m_position);
    } else {
        m_targetPosition = position;
    }
}
void Camera::setSize(const sf::Vector2f& size)
{
    m_size = size;
    m_view.setSize(m_size.x / m_zoom, m_size.y / m_zoom);
}

void Camera::setZoom(float zoom)
{
    if (zoom > 0.f)  // Prevent division by zero or negative zoom
    {
        m_zoom = zoom;
        m_view.setSize(m_size.x / m_zoom, m_size.y / m_zoom);
    }
}

void Camera::setSmoothness(float smoothness)
{
    if (smoothness >= 0.f)  // Prevent negative smoothness
    {
        m_smoothness = smoothness;
    }
}

void Camera::update(float deltaTime)
{
    // Update position with smooth movement
    if (m_smoothness > 0.f)
    {
        m_position += (m_targetPosition - m_position) * m_smoothness * deltaTime;
    }
    else
    {
        m_position = m_targetPosition;  // Instant movement if smoothness is 0
    }

    // Update view
    m_view.setCenter(m_position);
    m_view.setSize(m_size.x / m_zoom, m_size.y / m_zoom);
}

void Camera::moveToNextPart(int dx, int dy, bool teleport) {
    sf::Vector2f newPosition = m_targetPosition;
    newPosition.x += dx * m_size.x;
    newPosition.y += dy * m_size.y;
    setPosition(newPosition, teleport);
}
bool Camera::isMoving() const {
    // Compare current position with target position
    // Using a small epsilon value to account for floating-point precision
    const float epsilon = 0.01f;
    return (std::abs(m_position.x - m_targetPosition.x) > epsilon ||
            std::abs(m_position.y - m_targetPosition.y) > epsilon);
}
void Camera::applyTo(sf::RenderWindow& window)
{
    window.setView(m_view);
}