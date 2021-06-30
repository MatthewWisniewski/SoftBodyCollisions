#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>

class Wall {
    public:
        sf::Vector2f startPos;
        sf::Vector2f endPos;

        sf::Vector2f normalisedVector;

        float length;

        Wall(sf::Vector2f startPos, sf::Vector2f endPos);
        void renderWall(sf::RenderWindow *window);

        sf::Vector2f getVectorForm();

    private:
        void calculateLength();
};

#endif //WALL_H

