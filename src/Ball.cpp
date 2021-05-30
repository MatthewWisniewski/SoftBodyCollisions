//
// Created by Matthew on 28/05/2021.
//

#include <vector>
#include <SFML/Graphics.hpp>
#include "ball.h"

void Ball :: setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
    render.setPosition(position);
}

void Ball :: setVelocity(float x, float y) {
    velocity = sf::Vector2f(x, y);
}

void Ball :: setUnbalancedForce(float x, float y) {
    unbalancedForce = sf::Vector2f(x, y);
}

void Ball :: addForce(sf::Vector2f newForce) {
    unbalancedForce += newForce;
}

void Ball :: applyGravity(sf::Vector2f g) {
    unbalancedForce += mass * g;
}

void Ball :: setMass(float mass) {
    this->mass = mass;
}

void Ball :: applyTimeStep(float deltaTime) {
    sf::Vector2f acceleration = unbalancedForce / mass;
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    render.setPosition(position);
    setUnbalancedForce(0,0);
}