#include <SFML/Graphics.hpp>
#include "Wall.h"

class Ray {
    public:
        sf::Vector2f origin;
        sf::Vector2f destination;

        sf::Vector2f direction;
        float magnitude;

        bool intersected;

        Ray(sf::Vector2f origin, sf::Vector2f direction);

        bool updateNearestIntersection(Wall *wall);
        void resetDestination();

        void render(sf::RenderWindow *window);
};

