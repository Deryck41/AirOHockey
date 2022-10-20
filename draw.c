#include <gl/gl.h>
#include <math.h>
#include "gameobjects.h"

void DrawCircle(int accuracy){
    float x, y;
    float da = M_PI * 2.0 / accuracy;

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0);

        for (int i = 0; i <= accuracy; i++){
            glVertex2f(sin (da * i), cos (da * i));
        }

    glEnd();
}

void DrawBit(Bit obj){
    glPushMatrix();
        glTranslatef(obj.x, obj.y, 0);
        glScalef(obj.radius, obj.radius, 1);
        glColor3f(0.694,0.843,1);
        DrawCircle(70);
        glScalef(obj.radius*4.5, obj.radius*4.5, 1);
        glColor3f(0,0.165,0.616);
        DrawCircle(70);
        glColor3f(0,0,0.024);
        glScalef(obj.radius*3.5, obj.radius*3.5, 1);
        DrawCircle(70);
        glColor3f(0.176,0.408,1);
        glScalef(obj.radius*3.1, obj.radius*3.1, 1);
        DrawCircle(70);

    glPopMatrix();
}

void DrawPuck(Puck obj){
    glPushMatrix();
        glTranslatef(obj.x, obj.y, 0);
        glScalef(obj.radius, obj.radius, 1);
        DrawCircle(50);
    glPopMatrix();
}
