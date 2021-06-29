#include "Wall.h"

#include <SFML/Graphics.hpp>
#include <math.h>

Wall::Wall(sf::Vector2f startPos, sf::Vector2f endPos) {
    this->startPos = startPos;
    this->endPos = endPos;
    this->normalisedVector = endPos - startPos;
    calculateLength();
}

sf::Vector2f Wall::getVectorForm() {
    return normalisedVector * length;
}

void Wall::calculateLength() {
    length = sqrt(pow((startPos.x - endPos.x),2) + pow((startPos.y - endPos.y), 2));
}

void Wall::renderWall(sf::RenderWindow *window) {
    window->draw(new sf::Vertex[2] {startPos, endPos}, 2, sf::Lines);
}
