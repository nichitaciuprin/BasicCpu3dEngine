// raylib has inverted x axis... WTF?

#include <string.h>
#include <iostream>
#include <math.h>

#include <string>

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define WINDOW_SCALE 10

void CreateWindow()
{
    // TODO
    // FLAG_VSYNC_HINT seems to work
    // but, creates enforses target framerate and brakes main loop
    // SetConfigFlags(ConfigFlags::FLAG_VSYNC_HINT);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CustomGameEngine");
	rlDisableBackfaceCulling();
    HideCursor();

    // Fullscreen
    // auto monitor = Raylib::GetCurrentMonitor();
    // auto width = Raylib::GetMonitorWidth(monitor);
    // auto height = Raylib::GetMonitorHeight(monitor);
    // Raylib::SetWindowSize(width, height);
    // Raylib::ToggleFullscreen();
}
void UpdateWindow()
{
    PollInputEvents();
}
void DestroyWindow()
{
    CloseWindow();
}
bool WindowQuit()
{
    return WindowShouldClose();
}

Vector2 GetWasdInput()
{
    int x = 0;
    int y = 0;
    if (IsKeyDown(KEY_W)) y++;
    if (IsKeyDown(KEY_A)) x--;
    if (IsKeyDown(KEY_S)) y--;
    if (IsKeyDown(KEY_D)) x++;
    return Vector2Normalize({ (float)x, (float)y });
}
Vector2 GetArrowInput()
{
    int x = 0;
    int y = 0;
    if (IsKeyDown(KEY_UP))    y++;
    if (IsKeyDown(KEY_LEFT))  x--;
    if (IsKeyDown(KEY_DOWN))  y--;
    if (IsKeyDown(KEY_RIGHT)) x++;
    return Vector2Normalize({ (float)x, (float)y });
}
Vector2 GetMouseInput()
{
    auto sensitivity = 0.005f;
    auto delta = GetMouseDelta();
    auto deltaX = delta.x * sensitivity;
    auto deltaY = delta.y * sensitivity;

    // moving mouse right creates negative Y
    // maybe its done to simplify camera rotation
    // but I dont like it
    deltaY = -deltaY;

    return { deltaX, deltaY };
}

void DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Color color)
{
    v1.x = -v1.x;
    v2.x = -v2.x;
    v3.x = -v3.x;
    DrawTriangle3D(v1, v2, v3, color);
}
void DrawLine(Vector3 v1, Vector3 v2, Color color)
{
    v1.x = -v1.x;
    v2.x = -v2.x;
    DrawLine3D(v1, v2, color);
}
void DrawGroundPlane(Vector3 position, Vector2 size, Color color)
{
    position.x = -position.x;
    DrawPlane(position, size, color);
}
void DrawSphere2(Vector3 position, float radius, Color color)
{
    position.x = -position.x;
    DrawSphere(position, radius, color);
}
void DrawSphereWire(Vector3 position, float radius, Color color)
{
    position.x = -position.x;
    DrawSphereWires(position, radius, 2, 6, color);
}
void DrawBound(Vector3 position, Vector3 size, Color color)
{
    position.x = -position.x;
    DrawCubeV(position, size, color);
}
void DrawCylinder(Vector3 position, float radius, float height, Color color)
{
    position.x = -position.x;
    DrawCylinder(position, radius, radius, height, 10, color);
}

void BeginDraw(int cpu)
{
    ClearBackground(BLACK);

    char msg[7] = "CPU 00";

    if (cpu > 99)
    {
        msg[5] = '9';
        msg[6] = '9';
    }
    else if (cpu > 9)
    {
        snprintf(msg+4, 3, "%d", cpu);
        // itoa(cpu, msg+4, 10);
    }
    else if (cpu > 0)
    {
        snprintf(msg+5, 2, "%d", cpu);
        // itoa(cpu, msg+5, 10);
    }

    Color color = cpu >= 50 ? RED : LIGHTGRAY;
    DrawText(msg, 110, 0, 20, color);

    DrawFPS(0, 0);
    SetMousePosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    BeginDrawing();
}
void EndDraw()
{
    EndDrawing();
}

void StartMode3d(Vector3 position, Vector3 target, Vector3 up, float fovy, int projection)
{
    Camera3D camera3d;

    camera3d.position = position;
    camera3d.target = target;
    camera3d.up = up;
    camera3d.fovy = fovy;
    camera3d.projection = projection;

    BeginMode3D(camera3d);
}
void FinishMode3d()
{
    EndMode3D();
}

bool IsKeyPressedRl(int key)
{
    return IsKeyPressed(key);
}
bool IsKeyDownRl(int key)
{
    return IsKeyDown(key);
}
