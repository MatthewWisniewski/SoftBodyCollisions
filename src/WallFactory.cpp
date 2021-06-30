#include "WallFactory.h"

Wall WallFactory(float start_x, float start_y, float end_x, float end_y) {
    return Wall(sf::Vector2f(start_x, start_y), sf::Vector2f(end_x, end_y));
}
