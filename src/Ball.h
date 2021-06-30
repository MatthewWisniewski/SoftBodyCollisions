#ifndef BALL_H
#define BALL_H

#include <vector>
#include <SFML/Graphics.hpp>

class Ball {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f unbalancedForce;

    float radius;
    float mass;

    sf::CircleShape render;

    Ball(float radius, float mass, sf::Vector2f position);

    void applyTimeStep(float deltaTime);

    void setMass(float mass);
    void setPosition(float x, float y);
    void setVelocity(float x, float y);
    void setUnbalancedForce(float x, float y);
    void addForce(sf::Vector2f newForce);

    void applyGravity(sf::Vector2f g);
};

#endif //BALL_H
