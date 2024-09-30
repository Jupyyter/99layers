#pragma once
class Object
    {
    public:
        Object(int x, int y, int w, int h, std::string tname);
        ~Object() = default;

        void draw(sf::RenderWindow &window)const;

        sf::RectangleShape rect;
        sf::Texture tex;
        std::string texid;
    };