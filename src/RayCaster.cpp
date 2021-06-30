#include "RayCaster.h"
#include <iostream>

//TODO: Add error handling

RayCaster::RayCaster(sf::Vector2f position, std::vector<Wall *> walls) {
    this->position = position;
    this->walls = walls;
}

void RayCaster::addRayInDirection(sf::Vector2f direction) {
    rays.push_back(new Ray(position, direction));
    updateRay(rays.size() - 1);
}

void RayCaster::updateRay(int rayIndex) {
    for (int i = 0; i < walls.size(); i++) {
        rays[rayIndex]->updateNearestIntersection(walls[i]);
    }
}

void RayCaster::updateRays() {
    for (int i = 0; i < rays.size(); i++) {
        updateRay(i);
    }
}

void RayCaster::render(sf::RenderWindow *window) {
    sf::CircleShape originRender(6);
    originRender.setOrigin(6, 6);
    originRender.setPosition(position.x, position.y);
    originRender.setFillColor(sf::Color::Red);

    for (Ray *ray : rays) {
        ray->render(window);
    }

    window->draw(originRender);
}

void RayCaster::setPosition(sf::Vector2f newPosition) {
    position = newPosition;
    for (Ray *ray : rays) {
        ray->setOrigin(position);
    }
    updateRays();
}

sf::Vector2f RayCaster::getPosition() {
    return position;
}

