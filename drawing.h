#ifndef FILEDRAWING_H
#define FILEDRAWING_H

void InitTexture(int id, char * path, unsigned int textures[]);

void DrawBackground(float xFactor, unsigned int textures[]);

void DrawPuck(float puckX, float puckY, float puckRadius);

void DrawBit(float bitX, float bitY, float bitRadius, unsigned int textures[]);

#endif