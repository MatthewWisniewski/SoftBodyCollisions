#include <SFML/Graphics.hpp>
#include <vector>
#include "ball.h"

const int WIDTH = 640;
const int HEIGHT = 480;

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "WORK IN PROGRESS");
    window.setFramerateLimit(50);
    Ball ball = Ball();
    ball.render = sf::CircleShape(50);
    ball.render.setOrigin(ball.render.getRadius()/2,ball.render.getRadius()/2);

    ball.setMass(1);

    ball.setPosition(200,50);
    ball.setVelocity(0,0);
    ball.setUnbalancedForce(0,0);

    std::vector<sf::Shape*> shapes;

    bool keepGoing = false;

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
                    sf::CircleShape *shape = new sf::CircleShape(50);
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    shape->setOrigin(shape->getRadius(), shape->getRadius());
                    shape->setPosition((float) mousePos.x, (float) mousePos.y);
                    shape->setFillColor(sf::Color(100, 250, 50));
                    shapes.push_back(shape);
                }
            }
        }
        //TODO:Make this collision work properly
        if (keepGoing) {
            ball.applyGravity(sf::Vector2f(0, 9.8));
            ball.applyTimeStep(0.1);
            if (ball.position.y + ball.render.getRadius() >= HEIGHT) {
                ball.velocity.y *= -0.9;
            }
        }

        window.clear();

//        for(auto it=shapes.begin();it!=shapes.end();it++)
//        {
//            window.draw(**it);
//        }
        window.draw(ball.render);
        window.display();
    }

    return 0;
}