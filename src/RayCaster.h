#include "Ray.h"

class RayCaster {
    private:
        std::vector<Ray *> rays;
        sf::Vector2f position;
        std::vector<Wall *> walls;

        void updateRay(int rayIndex);
        void updateRays();

    public:
        RayCaster(sf::Vector2f position, std::vector<Wall *> walls);
        void addRayInDirection(sf::Vector2f direction);
        void render(sf::RenderWindow *window);

        sf::Vector2f getPosition();
        void setPosition(sf::Vector2f newPosition);
};

