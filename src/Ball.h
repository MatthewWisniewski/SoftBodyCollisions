#include <vector>
#include <SFML/Graphics.hpp>

class Ball {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f unbalancedForce;

    float mass;

    sf::CircleShape render;

    void applyTimeStep(float deltaTime);

    void setMass(float mass);
    void setPosition(float x, float y);
    void setVelocity(float x, float y);
    void setUnbalancedForce(float x, float y);
    void addForce(sf::Vector2f newForce);

    void applyGravity(sf::Vector2f g);
};
