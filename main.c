#include <winsock2.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <gl/gl.h>
#include <pthread.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define WIN_WIDTH 1200
#define WIN_HEIGHT 700
#define PORT 4445
#define CLIENT TRUE
#define SERVER FALSE


WSADATA ws;

float xFactor;


typedef struct{
    float x, y;
    float speedX, speedY;
    float radius;
} Puck;

typedef struct{
    float x, y;
    float radius;
} Bit;


Puck puck;
Bit userBit;
Bit user2Bit;











// Physics




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

void ReflexCursor(HWND hwnd, float glX, float glY){
    POINT cursor;

    cursor.x = (glX + xFactor) * (double) WIN_WIDTH * 2 * xFactor;
    cursor.y  = (((glY + 1) / 2 - 1) / -1) * (double) WIN_HEIGHT;
    ClientToScreen(hwnd, &cursor);
    SetCursorPos(cursor.x,cursor.y);
}

void ReflexBit(Bit *obj, BOOL role){
    if (obj->y < -1 + obj->radius)
        obj->y = -1 + obj->radius;

    if (obj->y > 1 - obj->radius)
        obj->y = 1 - obj->radius;

    if (role == CLIENT){
        if (obj->x < obj->radius - xFactor)
            obj->x = obj->radius - xFactor;

        if (obj->x > 0 - obj->radius)
            obj->x = 0 - obj->radius;
    }
    else{
        if (obj->x < obj->radius)
            obj->x = obj->radius;

        if (obj->x > xFactor - obj->radius)
            obj->x = xFactor - obj->radius;
    
    }
}

void MoveBitTo(Bit *obj, float x, float y){
    obj->x = x;
    obj->y = y;

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








// Drawing


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

void DrawSprite(unsigned int texture, float vertex[]);

void DrawBit(Bit obj){
    glPushMatrix();
        glTranslatef(obj.x, obj.y, 0);
        glScalef(obj.radius, obj.radius, 1);
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
        DrawSprite(1, vertex);

    glPopMatrix();
}

void DrawPuck(Puck obj){
    glPushMatrix();
        glTranslatef(obj.x, obj.y, 0);
        glScalef(obj.radius, obj.radius, 1);
        DrawCircle(50);
    glPopMatrix();
}

void DrawBackground(){
    glPushMatrix();
        // glColor3f(0.471,0.486,0.486);
        // glScalef(0.5, 0.5, 0);
        // DrawCircle(60);

        // glColor3f(0, 0, 0);
        // glScalef(0.92, 0.92, 0);
        // DrawCircle(60);
        float vertex[] = {-1 * xFactor,-1,0, xFactor,-1,0, xFactor,1,0, -1 * xFactor,1,0};
        DrawSprite(2, vertex);
        

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

unsigned int textures[3];

void InitTexture(int id, char * path){
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



float texCoords[] = {0,0, 1,0, 1,1, 0,1};

void DrawSprite(unsigned int texture, float vertex[]){
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








void InitPuck(Puck *obj, float xObj, float yObj, float speedXObj, float speedYObj, float radiusObj){
    obj->x = xObj;
    obj->y = yObj;
    obj->speedX = speedXObj;
    obj->speedY = speedYObj;
    obj->radius = radiusObj;

}

void InitBit(Bit *obj, float xObj, float yObj, float radiusObj){
    obj->x = xObj;
    obj->y = yObj;
    obj->radius = radiusObj;
}



void GameInit(){
    glGenTextures(2, textures);

    InitTexture(1, "resources/bit.png");
    InitTexture(2, "resources/table.png");
    InitPuck(&puck, 0, 0, 0, 0, 0.11);
    InitBit(&userBit, -0.4, 0, 0.14);
    InitBit(&user2Bit, 0.4, 0, 0.14);
}


void DrawFrame(){
    
    DrawBackground();

    glColor3f(1, 0, 0);
    DrawPuck(puck);
    DrawBit(userBit);
    DrawBit(user2Bit);
}

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);

void DisableOpenGL(HWND, HDC, HGLRC);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    WSAStartup(MAKEWORD(2, 2), &ws);

    SOCKET sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN socketSettings;
    memset(&socketSettings, 0, sizeof(socketSettings));
    socketSettings.sin_family = AF_INET;
    socketSettings.sin_port = htons(PORT);

    printf("1) server\n2) client\n");
    int choise;
    BOOL role;
    scanf("%1i", &choise);

    SOCKET clientSocket;
    SOCKADDR_IN clientAddres;

    if (choise == 2){
        role = CLIENT;

        char ip[16];
        scanf("%s", ip);

        socketSettings.sin_addr.S_un.S_addr = inet_addr(ip);

        if(connect(sock, &socketSettings, sizeof(socketSettings)) != 0)
            printf("Error\n\n\n%i", WSAGetLastError());

    }
    else{
        role = SERVER;

        bind(sock, &socketSettings, sizeof(socketSettings));
        listen(sock, 100);

        int clientAddresSize = sizeof(clientAddres);

        clientSocket = accept(sock, &clientAddres, &clientAddresSize);
        printf("connect OK\n");
    }



    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "AeroHockey",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          WIN_WIDTH,
                          WIN_HEIGHT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, SW_SHOWNORMAL);
    EnableOpenGL(hwnd, &hDC, &hRC);


    xFactor = (float) WIN_WIDTH / WIN_HEIGHT;   


    glScalef(1 / xFactor, 1, 1);
    GameInit();

    POINT userPoint;
    Bit *roleBitPointer;

    if (role == CLIENT){
        roleBitPointer = &user2Bit;
    }
    else{
        roleBitPointer = &userBit;
    }

    

    float clientData[2];
    float serverData[4];

    void *networkBroadcast(){
        while (TRUE){
            if (role == CLIENT){
                    clientData[0] = user2Bit.x;
                    clientData[1] = user2Bit.y;

                    send(sock, clientData, sizeof(clientData), 0);

                    memset(&serverData, 0, sizeof(serverData));
                    recv(sock, serverData, sizeof(serverData), 0);
                    
                    puck.x = serverData[0];
                    puck.y = serverData[1];
                    userBit.x = serverData[2];
                    userBit.y = serverData[3];
                }

                else{
                    
                    memset(&clientData, 0, sizeof(clientData));
                    recv(clientSocket, clientData, sizeof(clientData), 0);


                    serverData[0] = puck.x;
                    serverData[1] = puck.y;
                    serverData[2] = userBit.x;
                    serverData[3] = userBit.y;

                    send(clientSocket, serverData, sizeof(serverData), 0);
                }
        }
    }


    pthread_t thread;
    int statusAddr;
    int status = pthread_create(&thread, NULL, networkBroadcast, NULL);

    //pthread_join(thread, (void**)&statusAddr);

    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {


            GetCursorPos(&userPoint);
            ScreenToClient(hwnd, &userPoint);

            if (role == SERVER){
                MovePuck(&puck);
                MoveBitTo(&userBit,
                    userPoint.x / (double) WIN_WIDTH * (xFactor - (-1 * xFactor)) + (-1 * xFactor),
                    2 * (1 - userPoint.y / (double) WIN_HEIGHT) - 1);
                MoveBitTo(&user2Bit, clientData[0], clientData[1]);
            }
            else{
                user2Bit.x = userPoint.x / (double) WIN_WIDTH * 2 * xFactor - xFactor,
                user2Bit.y = 2 * (1 - userPoint.y / (double) WIN_HEIGHT) - 1;
            }
            
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            DrawFrame();

            SwapBuffers(hDC);

            Sleep (5);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_SETCURSOR && LOWORD(lParam) == HTCLIENT){
    SetCursor(NULL);

    return TRUE;
    }

    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}