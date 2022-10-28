#include <winsock2.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <gl/gl.h>
#include <pthread.h>

#include "physics.h"
#include "drawing.h"


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

unsigned int textures[5];

void GameInit(){
    glGenTextures(2, textures);

    InitTexture(1, "resources/bit.png", textures);
    InitTexture(2, "resources/table.png", textures);
    InitPuck(&puck, 0, 0, 0, 0, 0.11);
    InitBit(&userBit, -0.4, 0, 0.14);
    InitBit(&user2Bit, 0.4, 0, 0.14);
}


void DrawFrame(){
    
    DrawBackground(xFactor, textures);

    glColor3f(1, 0, 0);
    DrawPuck(puck.x, puck.y, puck.radius);
    DrawBit(userBit.x, userBit.y, userBit.radius, textures);
    DrawBit(user2Bit.x, user2Bit.y, user2Bit.radius, textures);
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
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


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

        if(connect(sock, (struct sockaddr*)&socketSettings, sizeof(socketSettings)) != 0)
            printf("Error\n\n\n%i", WSAGetLastError());

    }
    else{
        role = SERVER;

        bind(sock, (struct sockaddr*)&socketSettings, sizeof(socketSettings));
        listen(sock, 100);

        int clientAddresSize = sizeof(clientAddres);

        clientSocket = accept(sock, (struct sockaddr*)&clientAddres, &clientAddresSize);
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


    void clientNetworkBroadcast(){
        while (TRUE){
            clientData[0] = user2Bit.x;
            clientData[1] = user2Bit.y;
            send(sock, (const char *)clientData, sizeof(clientData), 0);
            memset(&serverData, 0, sizeof(serverData));
            recv(sock, (char *)serverData, sizeof(serverData), 0);
            puck.x = serverData[0];
            puck.y = serverData[1];
            userBit.x = serverData[2];
            userBit.y = serverData[3];
        }

    }

    void serverNetworkBroadcast(){
        while (TRUE){
            memset(&clientData, 0, sizeof(clientData));
            recv(clientSocket, (char *)clientData, sizeof(clientData), 0);
            user2Bit.x = clientData[0];
            user2Bit.y = clientData[1];
            serverData[0] = puck.x;
            serverData[1] = puck.y;
            serverData[2] = userBit.x;
            serverData[3] = userBit.y;
            send(clientSocket, (const char *)serverData, sizeof(serverData), 0);
        }
    }

    pthread_t thread;
    int statusAddr;
    void (*roleNetworkBroadcast)();

    if (role == CLIENT){
        roleNetworkBroadcast = clientNetworkBroadcast;
    }
    else{
        roleNetworkBroadcast = serverNetworkBroadcast;
    }

    int status = pthread_create(&thread, NULL, roleNetworkBroadcast, NULL);

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

            if (role == CLIENT){
                user2Bit.x = userPoint.x / (double) WIN_WIDTH * 2 * xFactor - xFactor,
                user2Bit.y = 2 * (1 - userPoint.y / (double) WIN_HEIGHT) - 1;
            }
            else{
                MoveBitTo(&userBit.x, &userBit.y, &userBit.radius, &puck.x, &puck.y, &puck.speedX, &puck.speedY, &puck.radius,
                    userPoint.x / (double) WIN_WIDTH * (xFactor - (-1 * xFactor)) + (-1 * xFactor),
                  2 * (1 - userPoint.y / (double) WIN_HEIGHT) - 1);
                MoveBitTo(&user2Bit.x, &user2Bit.y, &user2Bit.radius, &puck.x, &puck.y, &puck.speedX, &puck.speedY, &puck.radius,
                     user2Bit.x, user2Bit.y);
                MovePuck(&puck.x, &puck.y, &puck.speedX, &puck.speedY, &puck.radius, xFactor);

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