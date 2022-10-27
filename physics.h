#ifndef FILEPHYSICS_H_
#define FILEPHYSICS_H_

void MoveBitTo(float *bitX, float *bitY, float *bitRadius, float *puckX,
 float *puckY, float *puckSpeedX, float *puckSpeedY, float *puckRadius, float x, float y);

void MovePuck(float *puckX, float *puckY, float *puckSpeedX, float *puckSpeedY, float *puckRadius, float xFactor);

#endif