#include <math.h>
#include <windows.h>
#include "gameobjects.h"

float gravity = 0.0006;
float reflectionFactor = 0.85;
float frictionFactor = 0.0001;

BOOL Colision(float x1, float y1, float r, float x2, float y2){
    return pow(x1 - x2, 2) + pow(y1 - y2, 2) < pow(r, 2);
}

void Repulse(Puck *obj, float x, float y, float speed){
    float vector = atan2(obj->speedX, obj->speedY);
    float colosionVector = atan2(obj->x - x, obj->y - y);

    float angleVector = speed == 0 ? M_PI - vector + 2 * colosionVector : colosionVector;
    speed = speed == 0 ? sqrt(pow(obj->speedX, 2) + pow(obj->speedY, 2)) : speed;

    obj->speedX = sin(angleVector) * speed;
    obj->speedY = cos(angleVector) * speed;
}

void MoveBitTo(Bit *obj, float x, float y){
    obj->x = x;
    obj->y = y;

    if (obj->y < -1 + obj->radius)
        obj->y = -1 + obj->radius;

    if (obj->y > 1 - obj->radius)
        obj->y = 1 - obj->radius;

    if (obj->x < obj->radius - xFactor)
        obj->x = obj->radius - xFactor;

    if (obj->x > xFactor - obj->radius)
        obj->x = xFactor - obj->radius;

    if (Colision(obj->x, obj->y, obj->radius + puck.radius, puck.x, puck.y)){
        Repulse(&puck, obj->x, obj->y, 0.09);
    }
}

void MovePuck(Puck *obj){
    obj->x += obj->speedX;
    obj->y += obj->speedY;
    //obj->speedY -= gravity;
    // Reflect(&obj->dy, &obj->y, (obj->y < -1 + obj->radius), obj->radius - 1);
    if (obj->y < -1 + obj->radius){
        obj->speedY *= -reflectionFactor;
        obj->y = -1 + obj->radius;
    }

    if (obj->y > 1 - obj->radius){
        obj->speedY *= -reflectionFactor;
        obj->y = 1 - obj->radius;
    }


    if (obj->x < obj->radius - xFactor){
        obj->speedX *= -reflectionFactor;
        obj->x = obj->radius - xFactor;
    }

    if (obj->x > xFactor - obj->radius){
        obj->speedX *= -reflectionFactor;
        obj->x = xFactor - obj->radius;
    }

    if (obj->speedX > 0)
        obj->speedX -= frictionFactor;

    if (obj->speedX < 0)
        obj->speedX += frictionFactor;

    if (obj->speedY > 0)
        obj->speedY -= frictionFactor;

    if (obj->speedY < 0)
        obj->speedY += frictionFactor;

    if ((obj->speedY <= frictionFactor && obj->speedY > 0) || (obj->speedY >= frictionFactor && obj->speedY < 0))
        obj->speedY = 0;

    if ((obj->speedX <= frictionFactor && obj->speedX > 0) || (obj->speedX >= frictionFactor && obj->speedX < 0))
        obj->speedX = 0;

}

void Reflect(float *speed, float *coord, BOOL condition, float wall){
    if (!condition)
        return;

    *speed *= - reflectionFactor;
    *coord = wall;
}