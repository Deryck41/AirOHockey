#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

BOOL Colision(float x1, float y1, float r, float x2, float y2);

void Repulse(Puck *obj, float x, float y, float speed);

void MoveBitTo(Bit *obj, float x, float y);

void MovePuck(Puck *obj);

void Reflect(float *speed, float *coord, BOOL condition, float wall);

#endif