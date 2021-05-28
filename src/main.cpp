#include <SFML/Graphics.hpp>
#include <vector>
#include "ball.h"
#include <math.h>

const int WIDTH = 640;
const int HEIGHT = 480;

float dotProduct(sf::Vector2f a, sf::Vector2f b) {
    return a.x*b.x + a.y*b.y;
}

class DampedSpring {
public:
    float restLength;
    float springConstant;
    float dampingFactor;

    Ball *a;
    Ball *b;

    DampedSpring(Ball *a, Ball *b, float restLength, float springConstant, float dampingFactor);
    void applyForces();
};

DampedSpring::DampedSpring(Ball *a, Ball *b, float restLength, float springConstant, float dampingFactor) {
    this->a = a;
    this->b = b;
    this->restLength = restLength;
    this->springConstant = springConstant;
    this->dampingFactor = dampingFactor;
}

void DampedSpring::applyForces() {
    float absDistance = sqrt(pow(a->position.x - b->position.x, 2) + pow(a->position.y - b->position.y, 2));

    float drivingForce = springConstant * (absDistance - restLength);
    float dampingForce = dampingFactor * dotProduct(((b->position - a->position) / absDistance), (b->velocity - a->velocity));

    float totalForce = drivingForce + dampingForce;

    a->addForce(totalForce / absDistance * (b->position - a->position));
    b->addForce(totalForce/absDistance * (a->position - b->position));
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
        window.draw(ball1.render);
        window.draw(ball2.render);

        //todo: Imrove rendering of springs
        sf::Vertex line[] = {ball1.position, ball2.position};
        window.draw(line,2,sf::Lines);
        window.display();
    }

    return 0;
}