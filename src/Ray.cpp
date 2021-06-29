#include <math.h>
#include <iostream>
#include "Ray.h"

//TODO: direction should always have magnitude greater than 0


Ray::Ray(sf::Vector2f origin, sf::Vector2f direction) {
    this->origin = origin;
    this->direction = direction;

    magnitude = -1.0f;
    intersected = false;
}

bool Ray::updateNearestIntersection(Wall *wall) {
    // Maths is taken from
    // https://ncase.me/sight-and-light/

    // If the two line segments are perpendicular, no intersection
    // can take place
    if (direction.y / direction.x == wall->normalisedVector.y / wall->normalisedVector.x) {
        return false;
    }
    // T1 is the multiplier of direction from the origin point to the point of intersection
    // T2 is the multiplier of the normalisedVector to go from wall->startPos to the point of intersection
    float T2 = (direction.x * (wall->startPos.y - origin.y) + direction.y*(origin.x - wall->startPos.x))
            / (wall->normalisedVector.x * direction.y - wall->normalisedVector.y * direction.x);
    float T1 = (wall->startPos.x + wall->normalisedVector.x * T2 - origin.x) / direction.x;

    // If the intersection occurs in the opposite direction to which the ray was
    // emitted, no update is made to the destination point
    if (T1 < 0) {
        return false;
    }

    // If the intersection point with the wall line occurs outside of the segment
    // that the wall consists of, then no update is made to the destination point
    if (T2 < 0 || T2 > 1) {
        return false;
    }

    float newMagnitude = T1 * sqrt(direction.x * direction.x + direction.y * direction.y);

    if (!intersected || newMagnitude < magnitude) {
        magnitude = newMagnitude;
        destination = origin + direction * T1;
        intersected = true;
        return true;
    } else {
        return false;
    }
}

void Ray::resetDestination() {
    intersected = false;
}

void Ray::render(sf::RenderWindow *window) {
    if (intersected) {

        sf::CircleShape originRender(4);
        originRender.setOrigin(4, 4);
        originRender.setPosition(origin.x, origin.y);
        originRender.setFillColor(sf::Color::Red);

        sf::CircleShape destRender(4);
        destRender.setOrigin(4, 4);
        destRender.setPosition(destination.x, destination.y);
        destRender.setFillColor(sf::Color::Red);

        sf::Vertex line[2]{origin, destination};
        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;
        window->draw(line, 2, sf::Lines);

        window->draw(originRender);
        window->draw(destRender);
    }
}