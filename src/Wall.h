#include <SFML/Graphics.hpp>

class Wall {
    public:
        sf::Vector2f startPos;
        sf::Vector2f endPos;
        float length;

        Wall(sf::Vector2f startPos, sf::Vector2f endPos);
        void renderWall(sf::RenderWindow *window);

    private:
        void calculateLength();
};

