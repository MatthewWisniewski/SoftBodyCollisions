#include<iostream>
#include<stdlib.h>

#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>

//#include "ball.h"
#include "DampedSpring.h"
#include "Ray.h"

const int WIDTH = 640;
const int HEIGHT = 480;

float TIME_PER_FRAME = 0.1;

int updatesPerFrame = 1;

void handleStaticCollision(Ball *a, Ball *b);
void handleDynamicCollision(Ball *a, Ball *b);

bool inCircle(float radius, sf::Vector2f origin, sf::Vector2f point) {
    return pow((origin.x-point.x), 2) + pow((origin.y - point.y), 2) <= radius*radius;
}

bool areBallsOverlapping(Ball *a, Ball*b) {
    return pow((a->position.x - b->position.x),2) + pow((a->position.y - b->position.y), 2) <= pow((a->radius + b->radius), 2);
}

float dProduct(sf::Vector2f a, sf::Vector2f b) {
    return a.x*b.x + a.y*b.y;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "WORK IN PROGRESS");
    window.setFramerateLimit(50);

    std::vector<Ball*> balls;

//    balls.push_back(new Ball(50 , 1, sf::Vector2f(200,50)));
//
//    balls.push_back(new Ball(50, 1, sf::Vector2f(400,300)));
//
//    for (int i = 0; i < 15; i++) {
//        balls.push_back(new Ball(rand() % 50 + 15, rand() % 15 + 1, sf::Vector2f(rand() % WIDTH, rand() % HEIGHT)));
//    }

    std::vector<DampedSpring*> springs;
    std::vector<Wall*> walls;

    //DampedSpring spring(balls[0], balls[1], 150, 0.1, 0.05);
    //springs.push_back(&spring);

    walls.push_back(new Wall(sf::Vector2f(0, 0), sf::Vector2f(0, HEIGHT)));
    walls.push_back(new Wall(sf::Vector2f(0, 0), sf::Vector2f(WIDTH, 0)));
    walls.push_back(new Wall(sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(0, HEIGHT)));
    walls.push_back(new Wall(sf::Vector2f(WIDTH, HEIGHT), sf::Vector2f(WIDTH, 0)));

    walls.push_back(new Wall(sf::Vector2f(50, 200), sf::Vector2f(250,300)));
    //walls.push_back(new Wall(sf::Vector2f(0, 200), sf::Vector2f(300,200)));

    std::vector<std::pair<Ball*, Ball*>> collidingPairs ;
    std::vector<Ball*> fakeBalls;

    bool keepGoing = false;

    Ball *selected = nullptr;

    sf::Vector2f mousePosition;

    //Build Soft body

    int startingIndex = balls.size();

    int NUM_ROWS = 3;
    int NUM_COLS = 5;

    //Initialise balls
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            sf::Vector2f position = sf::Vector2f(200+col*50, 50+row*50);
            balls.push_back(new Ball(20, 1, position));
        }
    }

    //Initialise Damped Springs
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            // Horizontal springs
            if (col != NUM_COLS - 1) {
                Ball *a = balls[startingIndex + row * NUM_COLS + col];
                Ball *b = balls[startingIndex + row * NUM_COLS + col + 1];
                springs.push_back(new DampedSpring(a, b, 50, 1.8, 1));
            }
            // Vertical springs
            if (row != NUM_ROWS - 1) {
                Ball *a = balls[startingIndex + row * NUM_COLS + col];
                Ball *b = balls[startingIndex + (row + 1) * NUM_COLS + col];
                springs.push_back(new DampedSpring(a, b, 50, 1.8, 1));
            }

            // Diagonal Springs
            if  (col != NUM_COLS - 1 && row != NUM_ROWS - 1) {
                Ball *a = balls[startingIndex + row * NUM_COLS + col];
                Ball *b = balls[startingIndex + (row + 1) * NUM_COLS + col+1];
                springs.push_back(new DampedSpring(a, b, 70, 3.6, 1));
            }
            if  (col != 0 && row != NUM_ROWS - 1) {
                Ball *a = balls[startingIndex + row * NUM_COLS + col];
                Ball *b = balls[startingIndex + (row + 1) * NUM_COLS + col - 1];
                springs.push_back(new DampedSpring(a, b, 70, 3.6, 1));
            }
        }
    }

    //RAY EXPERIMENTATION

    Ray ray = Ray(sf::Vector2f(300, 150), sf::Vector2f(1,1));

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
                        if (inCircle(balls[i]->radius, balls[i]->position, mousePosition)) {
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
            for (int increment = 0; increment < updatesPerFrame; increment++) {


                for (int i = 0; i < balls.size(); i++) {
                    balls[i]->applyTimeStep(TIME_PER_FRAME / updatesPerFrame);
                    balls[i]->applyGravity(sf::Vector2f(0, 9.8));
//                if (balls[i]->position.y + balls[i]->radius >= HEIGHT) {
//                    balls[i]->velocity.y *= -0.9;
//                }
                    if (selected == balls[i]) {
                        selected->position = mousePosition;
                    }
                }
                for (int i = 0; i < springs.size(); i++) {
                    springs[i]->applyForces();
                }

                //Wall Collisions
                for (int i = 0; i < balls.size(); i++) {
                    for (int j = 0; j < walls.size(); j++) {
                        //Identify the point on walls[j] that is closest to the position of balls[i]

                        float t = dProduct((walls[j]->endPos - walls[j]->startPos),
                                           balls[i]->position - walls[j]->startPos) /
                                  (walls[j]->length * walls[j]->length);
                        t = std::max(0.0f, std::min(1.0f, t));

                        sf::Vector2f closestPoint = walls[j]->startPos + t * (walls[j]->endPos - walls[j]->startPos);

                        //Handle static collision and set up to handle dynamic collision

                        if (inCircle(balls[i]->radius, balls[i]->position, closestPoint)) {
                            Ball fakeBall = Ball(0.5, 1000000 * balls[i]->mass, closestPoint);

                            fakeBalls.push_back(&fakeBall);
                            collidingPairs.push_back(std::make_pair(balls[i], &fakeBall));

                            float centerToWallDistance = sqrt(pow((balls[i]->position.x - closestPoint.x), 2) +
                                                              pow((balls[i]->position.y - closestPoint.y), 2));
                            float overlap = centerToWallDistance - balls[i]->radius - fakeBall.radius;

                            balls[i]->position -=
                                    overlap / centerToWallDistance * (balls[i]->position - fakeBall.position);
                        }
                    }
                }

                //Static Collisions
                for (int i = 0; i < balls.size(); i++) {
                    for (int j = i + 1; j < balls.size(); j++) {
                        if (areBallsOverlapping(balls[i], balls[j])) {
                            handleStaticCollision(balls[i], balls[j]);
                            collidingPairs.push_back(std::make_pair(balls[i], balls[j]));
                        }
                    }
                }
                //Dynamic Collisions
                for (std::pair<Ball *, Ball *> collidingBalls : collidingPairs) {
                    Ball *a = collidingBalls.first;
                    Ball *b = collidingBalls.second;
                    handleDynamicCollision(a, b);

                }
            }
        }


        window.clear();

        //RAY EXPERIMENTATION
        ray.resetDestination();
        sf::Vector2f deltaFromRay = mousePosition - ray.origin;
        sf::Vector2f normalisedDeltaFromRay = deltaFromRay /
                                              sqrt(deltaFromRay.x * deltaFromRay.x + deltaFromRay.y * deltaFromRay.y);
        ray.direction = normalisedDeltaFromRay;

        for (int i = 0; i < walls.size(); i++) {
            ray.updateNearestIntersection(walls[i]);
        }
        if (ray.intersected) {
            ray.render(&window);
        }

        for (int i = 0; i < walls.size(); i++) {
            walls[i]->renderWall(&window);
        }

        //todo: Improve rendering of springs
        for (int i = 0; i < springs.size(); i ++) {
            sf::Vertex line[] = {springs[i]->a->position, springs[i]->b->position};
            window.draw(line,2,sf::Lines);
        }

        for (int i = 0; i < balls.size(); i++) {
            window.draw(balls[i]->render);
        }

        window.display();
    }

    return 0;
}

void handleStaticCollision(Ball *a, Ball *b) {
    float dist = sqrt(
            pow((a->position.x - b->position.x), 2) + pow((a->position.y - b->position.y), 2));
    float halfOverlap = 0.5 * (dist - (a->radius + b->radius));

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
