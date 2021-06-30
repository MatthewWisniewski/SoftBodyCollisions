#ifndef RAY_H
#define RAY_H

#include <SFML/Graphics.hpp>
#include "Wall.h"

class Ray {
    private:
        sf::Vector2f origin;
        sf::Vector2f destination;

        sf::Vector2f direction;
        float magnitude;

        bool intersected;

    public:
        Ray(sf::Vector2f origin, sf::Vector2f direction);

        bool updateNearestIntersection(Wall *wall);
        void resetDestination();

        void render(sf::RenderWindow *window);

        sf::Vector2f getOrigin();
        void setOrigin(sf::Vector2f origin);

        sf::Vector2f getDestination();
        void setDestination(sf::Vector2f destination);

        sf::Vector2f getDirection();
        void setDirection(sf::Vector2f direction);

        bool hasDestination();
};

#endif //RAY_H

