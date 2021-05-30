#include "ball.h"

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
