#include<iostream>

#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>

//#include "ball.h"
#include "DampedSpring.h"
#include "Wall.h"

const int WIDTH = 640;
const int HEIGHT = 480;

void handleStaticCollision(Ball *a, Ball *b);
void handleDynamicCollision(Ball *a, Ball *b);

bool inCircle(float radius, sf::Vector2f origin, sf::Vector2f point) {
    return pow((origin.x-point.x), 2) + pow((origin.y - point.y), 2) <= radius*radius;
}

bool areBallsOverlapping(Ball *a, Ball*b) {
    return pow((a->position.x - b->position.x),2) + pow((a->position.y - b->position.y), 2) <= pow((a->render.getRadius() + b->render.getRadius()), 2);
}

float dProduct(sf::Vector2f a, sf::Vector2f b) {
    return a.x*b.x + a.y*b.y;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "WORK IN PROGRESS");
    window.setFramerateLimit(50);

    std::vector<Ball*> balls;

    balls.push_back(new Ball());
    balls[0]->render = sf::CircleShape(50);
    balls[0]->render.setOrigin(50,50);
    balls[0]->setMass(1);
    balls[0]->setPosition(200, 50);
    balls[0]->setVelocity(0, 0);
    balls[0]->setUnbalancedForce(0, 0);

    balls.push_back(new Ball());
    balls[1]->render = sf::CircleShape(50);
    balls[1]->render.setOrigin(balls[1]->render.getRadius(), balls[1]->render.getRadius());

    balls[1]->setMass(1);

    balls[1]->setPosition(400, 300);
    balls[1]->setVelocity(0, 0);
    balls[1]->setUnbalancedForce(0, 0);

    DampedSpring spring(balls[0], balls[1], 150, 0.1, 0.05);

    std::vector<Wall*> walls;

    walls.push_back(new Wall(sf::Vector2f(0, 0), sf::Vector2f(0, HEIGHT)));
    walls.push_back(new Wall(sf::Vector2f(0, 0), sf::Vector2f(WIDTH, 0)));
    walls.push_back(new Wall(sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(0, HEIGHT)));
    walls.push_back(new Wall(sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(WIDTH, 0)));

    walls.push_back(new Wall(sf::Vector2f(100, 200), sf::Vector2f(300,100)));
    //walls.push_back(new Wall(sf::Vector2f(0, 200), sf::Vector2f(300,200)));

    std::vector<std::pair<Ball*, Ball*>> collidingPairs ;
    std::vector<Ball*> fakeBalls;

    bool keepGoing = false;

    Ball *selected = nullptr;

    sf::Vector2f mousePosition;

    while (window.isOpen())
    {
        sf::Event event;
        mousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    return 0;
                }
                case sf::Event::KeyPressed:
                {
                    if (event.key.code == sf::Keyboard::P) {
                        keepGoing = !keepGoing;
                    }
                }
                case sf::Event::MouseButtonPressed:
                {
                    for (int i = 0; i < balls.size(); i++) {
                        if (inCircle(balls[i]->render.getRadius(), balls[i]->position, mousePosition)) {
                            selected = balls[i];
                            break;
                        }
                    }
                    if (selected != nullptr) {
                        selected->render.setFillColor(sf::Color::Blue);
                    }
                    break;
                }
                case sf::Event::MouseButtonReleased:
                {
                    if (selected != nullptr) {
                        selected->render.setFillColor(sf::Color::White);
                        selected = nullptr;
                    }
                }
            }
        }

        collidingPairs.clear();
        fakeBalls.clear();

        if (keepGoing) {
            //spring.applyForces();
            for (int i = 0; i < balls.size(); i++) {
                balls[i]->applyGravity(sf::Vector2f(0, 9.8));
                balls[i]->applyTimeStep(0.1);
//                if (balls[i]->position.y + balls[i]->render.getRadius() >= HEIGHT) {
//                    balls[i]->velocity.y *= -0.9;
//                }
                if (selected == balls[i]) {
                    selected->position = mousePosition;
                }
            }

            //Wall Collisions

            for (int i = 0; i < balls.size(); i++) {
                for (int j = 0; j < walls.size(); j++) {
                    //Identify the point on walls[j] that is closest to the position of balls[i]

                    float t = dProduct((walls[j]->endPos - walls[j]->startPos), balls[i]->position - walls[j]->startPos) / (walls[j]->length* walls[j]->length);

                    std::cout << t << std::endl;

                    t = std::max(0.0f, std::min(1.0f, t));

                    //float t = std::max(0.0f, std::min(1.0f, dProduct((walls[j]->endPos - walls[j]->startPos), (balls[i]->position - walls[j]->startPos))/walls[j]->length));

                    sf::Vector2f closestPoint = walls[j]->startPos + t * (walls[j]->endPos-walls[j]->startPos);

                    //Handle static collision and set up to handle dynamic collision

                    std::cout << closestPoint.x << " " << closestPoint.y << std::endl;

                    if (inCircle(balls[i]->render.getRadius(), balls[i]->position, closestPoint)) {
                        std::cout << "HELLO" << std::endl;
                        Ball fakeBall = Ball();
                        fakeBall.render = sf::CircleShape(0.5);
                        fakeBall.render.setOrigin(0.25, 0.25);
                        fakeBall.setMass(100 * balls[i]->mass);
                        fakeBall.setPosition(closestPoint.x, closestPoint.y);
                        fakeBall.setVelocity(0, 0);
                        fakeBall.setUnbalancedForce(0, 0);

                        fakeBalls.push_back(&fakeBall);
                        collidingPairs.push_back(std::make_pair(balls[i], &fakeBall));

                        float centerToWallDistance = sqrt(pow((balls[i]->position.x - closestPoint.x), 2) +  pow((balls[i]->position.y - closestPoint.y), 2));
                        float overlap = centerToWallDistance - balls[i]->render.getRadius() - fakeBall.render.getRadius();

                        balls[i]->position -= overlap / centerToWallDistance * (balls[i]->position - fakeBall.position);
                    }
                }
            }

            //Static Collisions
            for (int i = 0; i < balls.size(); i++) {
                for (int j = i+1; j < balls.size(); j++) {
                    if (areBallsOverlapping(balls[i], balls[j])) {
                        handleStaticCollision(balls[i], balls[j]);
                        collidingPairs.push_back(std::make_pair(balls[i], balls[j]));
                    }
                }
            }
            //Dynamic Collisions
            for (std::pair <Ball*, Ball*> collidingBalls : collidingPairs) {
                Ball *a = collidingBalls.first;
                Ball *b = collidingBalls.second;
                handleDynamicCollision(a, b);

            }
        }

        window.clear();

        for (int i = 0; i < walls.size(); i++) {
            walls[i]->renderWall(&window);
        }

        //todo: Improve rendering of springs
        sf::Vertex line[] = {balls[0]->position, balls[1]->position};
        window.draw(line,2,sf::Lines);

        window.draw(balls[0]->render);
        window.draw(balls[1]->render);

        window.display();
    }

    return 0;
}

void handleStaticCollision(Ball *a, Ball *b) {
    float dist = sqrt(
            pow((a->position.x - b->position.x), 2) + pow((a->position.y - b->position.y), 2));
    float halfOverlap = 0.5 * (dist - (a->render.getRadius() + b->render.getRadius()));

    sf::Vector2f staticCollisionDisplacement = halfOverlap * (b->position - a->position) / dist;
    a->position += staticCollisionDisplacement;
    b->position -= staticCollisionDisplacement;
}

void handleDynamicCollision(Ball *a, Ball *b) {
    float dist = sqrt(pow((a->position.x - b->position.x), 2) + pow((a->position.y - b->position.y), 2));

    sf::Vector2f normal = (b->position-a->position) / dist;

    sf::Vector2f tangential = sf::Vector2f(-normal.y, normal.x);

    float ball1TangentialResponse = dProduct(a->velocity, tangential);
    float ball2TangentialResponse = dProduct(b->velocity, tangential);

    float ball1NormalResponse = dProduct(a->velocity, normal);
    float ball2NormalResponse = dProduct(b->velocity, normal);

    //1D conservation of momentum (elastic collision)
    float totalMass = a->mass + b->mass;
    float m1 = (ball1NormalResponse * (a->mass - b->mass) + 2.0f * b->mass * ball2NormalResponse) / totalMass;
    float m2 = (ball2NormalResponse * (b->mass - a->mass) + 2.0f * a->mass * ball1NormalResponse) / totalMass;

    a->velocity = ball1TangentialResponse * tangential + normal * m1;
    b->velocity = ball2TangentialResponse * tangential + normal * m2;

//    a->velocity *= 0.995f;
//    b->velocity *= 0.995f;
}
