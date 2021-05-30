#include<iostream>

#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>

//#include "ball.h"
#include "DampedSpring.h"

const int WIDTH = 640;
const int HEIGHT = 480;


void HandleDynamicCollision(Ball *a, Ball *b);

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

    balls[1]->setMass(3);

    balls[1]->setPosition(400, 300);
    balls[1]->setVelocity(0, 0);
    balls[1]->setUnbalancedForce(0, 0);

    DampedSpring spring(balls[0], balls[1], 150, 0.1, 0.05);

    std::vector<std::pair<Ball*, Ball*>> collidingPairs ;

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
                    if (inCircle(balls[0]->render.getRadius(), balls[0]->position, mousePosition)) {
                        selected = balls[0];
                    } else if (inCircle(balls[1]->render.getRadius(), balls[1]->position, mousePosition)) {
                        selected = balls[1];
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

        if (keepGoing) {
            spring.applyForces();
            balls[0]->applyGravity(sf::Vector2f(0, 9.8));
            balls[1]->applyGravity(sf::Vector2f(0, 9.8));
            balls[0]->applyTimeStep(0.1);
            if (balls[0]->position.y + balls[0]->render.getRadius() >= HEIGHT) {
                balls[0]->velocity.y *= -0.9;
            }
            if (selected == balls[0]) {
                selected->position = mousePosition;
            }
            balls[1]->applyTimeStep(0.1);
            if (balls[1]->position.y + balls[1]->render.getRadius() >= HEIGHT) {
                balls[1]->velocity.y *= -0.9;
            }
            if (selected == balls[1]) {
                selected->position = mousePosition;
            }
            if (areBallsOverlapping(balls[0], balls[1])) {
                //Static Collision
                float dist = sqrt(
                        pow((balls[0]->position.x - balls[1]->position.x), 2) + pow((balls[0]->position.y - balls[1]->position.y), 2));
                float halfOverlap = 0.5 * (dist - (balls[0]->render.getRadius() + balls[1]->render.getRadius()));

                sf::Vector2f staticCollisionDisplacement = halfOverlap * (balls[1]->position - balls[0]->position) / dist;
                balls[0]->position += staticCollisionDisplacement;
                balls[1]->position -= staticCollisionDisplacement;
                collidingPairs.push_back(std::make_pair(balls[0], balls[1]));
            }
            for (std::pair <Ball*, Ball*> collidingBalls : collidingPairs) {
                Ball *a = collidingBalls.first;
                Ball *b = collidingBalls.second;
                HandleDynamicCollision(a, b);

            }
        }

        window.clear();

        //todo: Improve rendering of springs
        sf::Vertex line[] = {balls[0]->position, balls[1]->position};
        window.draw(line,2,sf::Lines);

        window.draw(balls[0]->render);
        window.draw(balls[1]->render);

        window.display();
    }

    return 0;
}

void HandleDynamicCollision(Ball *a, Ball *b) {
    float dist = sqrt(pow((a->position.x - b->position.x), 2) + pow((a->position.y - b->position.y), 2));

    //Dynamic Collision
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
}
