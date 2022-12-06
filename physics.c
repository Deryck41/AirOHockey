#include <math.h>
#include <windows.h>

float reflectionFactor = 0.85;
float frictionFactor = 0.0001;

BOOL Colision(float x1, float y1, float r, float x2, float y2){
    return pow(x1 - x2, 2) + pow(y1 - y2, 2) < pow(r, 2);
}

void Repulse(float *puckX, float *puckY, float *puckSpeedX, float *puckSpeedY, float x, float y, float speed){
    float vector = atan2(*puckSpeedX, *puckSpeedY);
    float colosionVector = atan2(*puckX - x, *puckY - y);

    float angleVector = speed == 0 ? M_PI - vector + 2 * colosionVector : colosionVector;
    speed = speed == 0 ? sqrt(pow(*puckSpeedX, 2) + pow(*puckSpeedY, 2)) : speed;

    *puckSpeedX = sin(angleVector) * speed;
    *puckSpeedY = cos(angleVector) * speed;
}

void ReflexCursor(HWND hwnd, float glX, float glY, float xFactor, int windowWidth, int windowHeight){
    POINT cursor;

    cursor.x = (glX + xFactor) * (double) windowWidth * 2 * xFactor;
    cursor.y  = (((glY + 1) / 2 - 1) / -1) * (double) windowHeight;
    ClientToScreen(hwnd, &cursor);
    SetCursorPos(cursor.x,cursor.y);
}

void ReflexBit(float *bitX, float *bitY, float *bitRadius, float xFactor, BOOL role){
    if (*bitY < -1 + *bitRadius)
        *bitY = -1 + *bitRadius;

    if (*bitY > 1 - *bitRadius)
        *bitY = 1 - *bitRadius;

    if (role == TRUE){
        if (*bitX < *bitRadius - xFactor)
            *bitX = *bitRadius - xFactor;

        if (*bitX > 0 - *bitRadius)
            *bitX = 0 - *bitRadius;
    }
    else{
        if (*bitX < *bitRadius)
            *bitX = *bitRadius;

        if (*bitX > xFactor - *bitRadius)
            *bitX = xFactor - *bitRadius;
    }

}

void CheckColision(float *bitX, float *bitY, float *bitRadius, float *puckX, float *puckY, float *puckSpeedX, float *puckSpeedY, float *puckRadius){
    if (Colision(*bitX, *bitY, *bitRadius + *puckRadius, *puckX, *puckY)){
        Repulse(puckX, puckY, puckSpeedX, puckSpeedY, *bitX, *bitY, 0.09);
    }
}

void MoveBitTo(float *bitX, float *bitY, float x, float y){
    *bitX = x;
    *bitY = y;


}

void MovePuck(float *puckX, float *puckY, float *puckSpeedX, float *puckSpeedY, float *puckRadius, float xFactor){
    *puckX += *puckSpeedX;
    *puckY += *puckSpeedY;

    if (*puckY < -1 + *puckRadius){
        *puckSpeedY *= -reflectionFactor;
        *puckY = -1 + *puckRadius;
    }

    if (*puckY > 1 - *puckRadius){
        *puckSpeedY *= -reflectionFactor;
        *puckY = 1 - *puckRadius;
    }


    if (*puckX < *puckRadius - xFactor){
        *puckSpeedX *= -reflectionFactor;
        *puckX = *puckRadius - xFactor;
    }

    if (*puckX > xFactor - *puckRadius){
        *puckSpeedX *= -reflectionFactor;
        *puckX = xFactor - *puckRadius;
    }

    if (*puckSpeedX > 0)
        *puckSpeedX -= frictionFactor;

    if (*puckSpeedX < 0)
        *puckSpeedX += frictionFactor;

    if (*puckSpeedY > 0)
        *puckSpeedY -= frictionFactor;

    if (*puckSpeedY < 0)
        *puckSpeedY += frictionFactor;

    if ((*puckSpeedY <= frictionFactor && *puckSpeedY > 0) || (*puckSpeedY >= frictionFactor && *puckSpeedY < 0))
        *puckSpeedY = 0;

    if ((*puckSpeedX <= frictionFactor && *puckSpeedX > 0) || (*puckSpeedX >= frictionFactor && *puckSpeedX < 0))
        *puckSpeedX = 0;

}

void Reflect(float *speed, float *coord, BOOL condition, float wall){
    if (!condition)
        return;

    *speed *= - reflectionFactor;
    *coord = wall;
}

short CheckGoal(float xFactor, float yFrom, float yTo, float puckX, float puckY, float puckRadius){

    // 0 - No Goal
    // 1 - Goal to server
    // 2 - Goal to client

    if (puckX >= xFactor - puckRadius && puckY >= yFrom && puckY <= yTo)
        return 1;
    else if(puckX <= (-1 * xFactor) + puckRadius && puckY >= yFrom && puckY <= yTo)
        return 2;
    return 0;
}


