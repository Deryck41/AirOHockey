#include <math.h>
#include <gl/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_easy_font.h"

void DrawCircle(int accuracy){
    float da = M_PI * 2.0 / accuracy;

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0);

        for (int i = 0; i <= accuracy; i++){
            glVertex2f(sin (da * i), cos (da * i));
        }

    glEnd();
}

float texCoords[] = {0,0, 1,0, 1,1, 0,1};

void DrawSprite(unsigned int texture, float vertex[], unsigned int textures[]){
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textures[texture]);
    glColor3f(1, 1, 1);
    glPushMatrix();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
}


void DrawBit(float bitX, float bitY, float bitRadius, unsigned int textures[]){
    glPushMatrix();
        glTranslatef(bitX, bitY, 0);
        glScalef(bitRadius, bitRadius, 1);
        // glColor3f(0.694,0.843,1);
        // DrawCircle(70);
        // glScalef(obj.radius*4.5, obj.radius*4.5, 1);
        // glColor3f(0,0.165,0.616);
        // DrawCircle(70);
        // glColor3f(0,0,0.024);
        // glScalef(obj.radius*3.5, obj.radius*3.5, 1);
        // DrawCircle(70);
        // glColor3f(0.176,0.408,1);
        // glScalef(obj.radius*3.1, obj.radius*3.1, 1);
        // DrawCircle(70);
        float vertex[] = {-1,-1,0, 1,-1,0, 1,1,0, -1,1,0};
        DrawSprite(1, vertex, textures);

    glPopMatrix();
}

void DrawPuck(float puckX, float puckY, float puckRadius){
    glPushMatrix();
        glTranslatef(puckX, puckY, 0);
        glScalef(puckRadius, puckRadius, 1);
        DrawCircle(50);
    glPopMatrix();
}

void DrawBackground(float xFactor, unsigned int textures[]){
    glPushMatrix();
        // glColor3f(0.471,0.486,0.486);
        // glScalef(0.5, 0.5, 0);
        // DrawCircle(60);

        // glColor3f(0, 0, 0);
        // glScalef(0.92, 0.92, 0);
        // DrawCircle(60);
        float vertex[] = {-1 * xFactor,-1,0, xFactor,-1,0, xFactor,1,0, -1 * xFactor,1,0};
        DrawSprite(2, vertex, textures);


    glPopMatrix();

    // glColor3f(0.471,0.486,0.486);
    // glLineWidth(8);

    // glBegin(GL_LINES);
    //     glVertex2f(0, 1);
    //     glVertex2f(0, -1);
    // glEnd();

    // glPushMatrix();

    //     glColor3f(0.471,0.486,0.486);
    //     glTranslatef(-1 * xFactor, 0, 0);
    //     glScalef(0.5, 0.5, 0);
    //     DrawCircle(60);

    //     glColor3f(0, 0, 0);
    //     glScalef(0.94, 0.94, 0);
    //     DrawCircle(60);

    // glPopMatrix();

    // glPushMatrix();

    //     glColor3f(0.471,0.486,0.486);
    //     glTranslatef(xFactor, 0, 0);
    //     glScalef(0.5, 0.5, 0);
    //     DrawCircle(60);

    //     glColor3f(0, 0, 0);
    //     glScalef(0.94, 0.94, 0);
    //     DrawCircle(60);

    // glPopMatrix();
}


void InitTexture(int id, char * path, unsigned int textures[]){
    int width, height, cnt;
    unsigned char *data = stbi_load(path, &width, &height, &cnt, 0);

    glBindTexture(GL_TEXTURE_2D, textures[id]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

void PrintString(float x, float y, char *text, float r, float g, float b){
    static char buffer[99999]; // ~500 chars
    int num_quads;

    num_quads = stb_easy_font_print(x, y, text, NULL, buffer, sizeof(buffer));

    glColor3f(r,g,b);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads*4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawScore(int first, int second){
    char firstScore[8];
    sprintf(firstScore, "%d", first);

    char secondScore[8];
    sprintf(secondScore, "%d", second);

    glPushMatrix();
        glTranslatef(-1.0 * (stb_easy_font_width(firstScore)/60.0), 0.9, 0);
        glScalef(0.015, -0.015, 1);
        
        PrintString(0, 0, firstScore, 1, 0, 0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-1.0 * (stb_easy_font_width(":")/330.0), 0.9, 0);
        glScalef(0.015, -0.015, 1);
        
        PrintString(0, 0, ":", 1, 0, 0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef((stb_easy_font_width(secondScore)/150.0), 0.9, 0);
        glScalef(0.015, -0.015, 1);
        
        PrintString(0, 0, secondScore, 1, 0, 0);
    glPopMatrix();
}