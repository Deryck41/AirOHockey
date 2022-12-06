/*
 * physics.h
 *
 *  Created on: 16 нояб. 2022 г.
 *      Author: EL
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

void MoveBitTo(float *bitX, float *bitY, float x, float y);

void MovePuck(float *puckX, float *puckY, float *puckSpeedX, float *puckSpeedY, float *puckRadius, float xFactor);

void ReflexBit(float *bitX, float *bitY, float *bitRadius, float xFactor, BOOL role);

void CheckColision(float *bitX, float *bitY, float *bitRadius, float *puckX, float *puckY, float *puckSpeedX, float *puckSpeedY, float *puckRadius);

short CheckGoal(float xFactor, float yFrom, float yTo, float puckX, float puckY, float puckRadius);

#endif /* PHYSICS_H_ */
