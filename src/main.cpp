#include<iostream>

#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>

//#include "ball.h"
#include "DampedSpring.h"

const int WIDTH = 640;
const int HEIGHT = 480;


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
    Ball ball1 = Ball();
    ball1.render = sf::CircleShape(50);
    ball1.render.setOrigin(ball1.render.getRadius(), ball1.render.getRadius());

    ball1.setMass(1);

    ball1.setPosition(200, 50);
    ball1.setVelocity(0, 0);
    ball1.setUnbalancedForce(0, 0);

    Ball ball2 = Ball();
    ball2.render = sf::CircleShape(50);
    ball2.render.setOrigin(ball2.render.getRadius(), ball2.render.getRadius());

    ball2.setMass(1);

    ball2.setPosition(400, 300);
    ball2.setVelocity(0, 0);
    ball2.setUnbalancedForce(0, 0);

    DampedSpring spring(&ball1, &ball2, 150, 0.1, 0.05);

    std::vector<sf::Shape*> shapes;

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
                    if (inCircle(ball1.render.getRadius(), ball1.position, mousePosition)) {
                        selected = &ball1;
                    } else if (inCircle(ball2.render.getRadius(), ball2.position, mousePosition)) {
                        selected = &ball2;
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
        //TODO:Make this collision work properly
        if (keepGoing) {
            spring.applyForces();
            ball1.applyGravity(sf::Vector2f(0, 9.8));
            ball2.applyGravity(sf::Vector2f(0, 9.8));
            ball1.applyTimeStep(0.1);
            if (ball1.position.y + ball1.render.getRadius() >= HEIGHT) {
                ball1.velocity.y *= -0.9;
            }
            if (selected == &ball1) {
                selected->position = mousePosition;
            }
            ball2.applyTimeStep(0.1);
            if (ball2.position.y + ball2.render.getRadius() >= HEIGHT) {
                ball2.velocity.y *= -0.9;
            }
            if (selected == &ball2) {
                selected->position = mousePosition;
            }
            if (areBallsOverlapping(&ball1, &ball2)) {
                //Static Collision
                float dist = sqrt(pow((ball1.position.x - ball2.position.x), 2) + pow((ball1.position.y - ball2.position.y), 2));
                float halfOverlap = 0.5 * (dist - (ball1.render.getRadius()+ball2.render.getRadius()));

                sf::Vector2f staticCollisionDisplacement = halfOverlap * (ball2.position - ball1.position) / dist;
                ball1.position += staticCollisionDisplacement;
                ball2.position -= staticCollisionDisplacement;

                //Dynamic Collision
                sf::Vector2f normal = (ball2.position-ball1.position) / dist;

                sf::Vector2f tangential = sf::Vector2f(-normal.y, normal.x);

                float ball1TangentialResponse = dProduct(ball1.velocity, tangential);
                float ball2TangentialResponse = dProduct(ball2.velocity, tangential);

                float ball1NormalResponse = dProduct(ball1.velocity, normal);
                float ball2NormalResponse = dProduct(ball2.velocity, normal);

                //1D conservation of momentum (elastic collision)
                float totalMass = ball1.mass + ball2.mass;
                float m1 = (ball1NormalResponse * (ball1.mass - ball2.mass) + 2.0f * ball2.mass * ball2NormalResponse) / totalMass;
                float m2 = (ball2NormalResponse * (ball2.mass - ball1.mass) + 2.0f * ball1.mass * ball1NormalResponse) / totalMass;

                ball1.velocity = ball1TangentialResponse * tangential + normal * m1;
                ball2.velocity = ball2TangentialResponse * tangential + normal * m2;
            }
        }

        window.clear();

//        for(auto it=shapes.begin();it!=shapes.end();it++)
//        {
//            window.draw(**it);
//        }
        //todo: Improve rendering of springs
        sf::Vertex line[] = {ball1.position, ball2.position};
        window.draw(line,2,sf::Lines);

        window.draw(ball1.render);
        window.draw(ball2.render);

        window.display();
    }

    return 0;
}