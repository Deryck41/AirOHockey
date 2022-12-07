/*
 * drawing.h
 *
 *  Created on: 16 нояб. 2022 г.
 *      Author: EL
 */

#ifndef DRAWING_H_
#define DRAWING_H_

void InitTexture(int id, char * path, unsigned int textures[]);


void DrawBackground(float xFactor, unsigned int textures[]);

void DrawPuck(float puckX, float puckY, float puckRadius);

void DrawBit(float bitX, float bitY, float bitRadius, unsigned int textures[]);

void DrawScore(int first, int second);

#endif /* DRAWING_H_ */
