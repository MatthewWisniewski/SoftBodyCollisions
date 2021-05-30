#include "math.h"
#include <vector>

#include "DampedSpring.h"

float dotProduct(sf::Vector2f a, sf::Vector2f b) {
    return a.x*b.x + a.y*b.y;
}

DampedSpring::DampedSpring(Ball *a, Ball *b, float restLength, float springConstant, float dampingFactor) {
    this->a = a;
    this->b = b;
    this->restLength = restLength;
    this->springConstant = springConstant;
    this->dampingFactor = dampingFactor;
}

void DampedSpring::applyForces() {
    float absDistance = sqrt(pow(a->position.x - b->position.x, 2) + pow(a->position.y - b->position.y, 2));

    float drivingForce = springConstant * (absDistance - restLength);
    float dampingForce = dampingFactor * dotProduct(((b->position - a->position) / absDistance), (b->velocity - a->velocity));

    float totalForce = drivingForce + dampingForce;

    a->addForce(totalForce / absDistance * (b->position - a->position));
    b->addForce(totalForce/absDistance * (a->position - b->position));
}