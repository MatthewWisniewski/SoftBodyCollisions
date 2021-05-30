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

    while (window.isOpen())
    {
        sf::Event event;
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
                    sf::Vector2f position = (sf::Vector2f) sf::Mouse::getPosition(window);
                    if (inCircle(ball1.render.getRadius(), ball1.position, position)) {
                        selected = &ball1;
                    } else if (inCircle(ball2.render.getRadius(), ball2.position, position)) {
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
            ball2.applyTimeStep(0.1);
            if (ball2.position.y + ball2.render.getRadius() >= HEIGHT) {
                ball2.velocity.y *= -0.9;
            }
        }

        window.clear();

//        for(auto it=shapes.begin();it!=shapes.end();it++)
//        {
//            window.draw(**it);
//        }
        //todo: Imrove rendering of springs
        sf::Vertex line[] = {ball1.position, ball2.position};
        window.draw(line,2,sf::Lines);

        window.draw(ball1.render);
        window.draw(ball2.render);

        window.display();
    }

    return 0;
}