// raylib has inverted x axis... WTF?

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define WINDOW_SCALE 10

struct Color
{
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
};

enum KeyboardKey
{
    KEY_NULL            = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
    KEY_APOSTROPHE      = 39,       // Key: '
    KEY_COMMA           = 44,       // Key: ,
    KEY_MINUS           = 45,       // Key: -
    KEY_PERIOD          = 46,       // Key: .
    KEY_SLASH           = 47,       // Key: /
    KEY_ZERO            = 48,       // Key: 0
    KEY_ONE             = 49,       // Key: 1
    KEY_TWO             = 50,       // Key: 2
    KEY_THREE           = 51,       // Key: 3
    KEY_FOUR            = 52,       // Key: 4
    KEY_FIVE            = 53,       // Key: 5
    KEY_SIX             = 54,       // Key: 6
    KEY_SEVEN           = 55,       // Key: 7
    KEY_EIGHT           = 56,       // Key: 8
    KEY_NINE            = 57,       // Key: 9
    KEY_SEMICOLON       = 59,       // Key: ;
    KEY_EQUAL           = 61,       // Key: =
    KEY_A               = 65,       // Key: A | a
    KEY_B               = 66,       // Key: B | b
    KEY_C               = 67,       // Key: C | c
    KEY_D               = 68,       // Key: D | d
    KEY_E               = 69,       // Key: E | e
    KEY_F               = 70,       // Key: F | f
    KEY_G               = 71,       // Key: G | g
    KEY_H               = 72,       // Key: H | h
    KEY_I               = 73,       // Key: I | i
    KEY_J               = 74,       // Key: J | j
    KEY_K               = 75,       // Key: K | k
    KEY_L               = 76,       // Key: L | l
    KEY_M               = 77,       // Key: M | m
    KEY_N               = 78,       // Key: N | n
    KEY_O               = 79,       // Key: O | o
    KEY_P               = 80,       // Key: P | p
    KEY_Q               = 81,       // Key: Q | q
    KEY_R               = 82,       // Key: R | r
    KEY_S               = 83,       // Key: S | s
    KEY_T               = 84,       // Key: T | t
    KEY_U               = 85,       // Key: U | u
    KEY_V               = 86,       // Key: V | v
    KEY_W               = 87,       // Key: W | w
    KEY_X               = 88,       // Key: X | x
    KEY_Y               = 89,       // Key: Y | y
    KEY_Z               = 90,       // Key: Z | z
    KEY_LEFT_BRACKET    = 91,       // Key: [
    KEY_BACKSLASH       = 92,       // Key: '\'
    KEY_RIGHT_BRACKET   = 93,       // Key: ]
    KEY_GRAVE           = 96,       // Key: `
    // Function keys
    KEY_SPACE           = 32,       // Key: Space
    KEY_ESCAPE          = 256,      // Key: Esc
    KEY_ENTER           = 257,      // Key: Enter
    KEY_TAB             = 258,      // Key: Tab
    KEY_BACKSPACE       = 259,      // Key: Backspace
    KEY_INSERT          = 260,      // Key: Ins
    KEY_DELETE          = 261,      // Key: Del
    KEY_RIGHT           = 262,      // Key: Cursor right
    KEY_LEFT            = 263,      // Key: Cursor left
    KEY_DOWN            = 264,      // Key: Cursor down
    KEY_UP              = 265,      // Key: Cursor up
    KEY_PAGE_UP         = 266,      // Key: Page up
    KEY_PAGE_DOWN       = 267,      // Key: Page down
    KEY_HOME            = 268,      // Key: Home
    KEY_END             = 269,      // Key: End
    KEY_CAPS_LOCK       = 280,      // Key: Caps lock
    KEY_SCROLL_LOCK     = 281,      // Key: Scroll down
    KEY_NUM_LOCK        = 282,      // Key: Num lock
    KEY_PRINT_SCREEN    = 283,      // Key: Print screen
    KEY_PAUSE           = 284,      // Key: Pause
    KEY_F1              = 290,      // Key: F1
    KEY_F2              = 291,      // Key: F2
    KEY_F3              = 292,      // Key: F3
    KEY_F4              = 293,      // Key: F4
    KEY_F5              = 294,      // Key: F5
    KEY_F6              = 295,      // Key: F6
    KEY_F7              = 296,      // Key: F7
    KEY_F8              = 297,      // Key: F8
    KEY_F9              = 298,      // Key: F9
    KEY_F10             = 299,      // Key: F10
    KEY_F11             = 300,      // Key: F11
    KEY_F12             = 301,      // Key: F12
    KEY_LEFT_SHIFT      = 340,      // Key: Shift left
    KEY_LEFT_CONTROL    = 341,      // Key: Control left
    KEY_LEFT_ALT        = 342,      // Key: Alt left
    KEY_LEFT_SUPER      = 343,      // Key: Super left
    KEY_RIGHT_SHIFT     = 344,      // Key: Shift right
    KEY_RIGHT_CONTROL   = 345,      // Key: Control right
    KEY_RIGHT_ALT       = 346,      // Key: Alt right
    KEY_RIGHT_SUPER     = 347,      // Key: Super right
    KEY_KB_MENU         = 348,      // Key: KB menu
    // Keypad keys
    KEY_KP_0            = 320,      // Key: Keypad 0
    KEY_KP_1            = 321,      // Key: Keypad 1
    KEY_KP_2            = 322,      // Key: Keypad 2
    KEY_KP_3            = 323,      // Key: Keypad 3
    KEY_KP_4            = 324,      // Key: Keypad 4
    KEY_KP_5            = 325,      // Key: Keypad 5
    KEY_KP_6            = 326,      // Key: Keypad 6
    KEY_KP_7            = 327,      // Key: Keypad 7
    KEY_KP_8            = 328,      // Key: Keypad 8
    KEY_KP_9            = 329,      // Key: Keypad 9
    KEY_KP_DECIMAL      = 330,      // Key: Keypad .
    KEY_KP_DIVIDE       = 331,      // Key: Keypad /
    KEY_KP_MULTIPLY     = 332,      // Key: Keypad *
    KEY_KP_SUBTRACT     = 333,      // Key: Keypad -
    KEY_KP_ADD          = 334,      // Key: Keypad +
    KEY_KP_ENTER        = 335,      // Key: Keypad Enter
    KEY_KP_EQUAL        = 336,      // Key: Keypad =
    // Android key buttons
    KEY_BACK            = 4,        // Key: Android back button
    KEY_MENU            = 82,       // Key: Android menu button
    KEY_VOLUME_UP       = 24,       // Key: Android volume up button
    KEY_VOLUME_DOWN     = 25        // Key: Android volume down button
};

const Color LIGHTGRAY  = { 200, 200, 200, 255 };  // Light Gray
const Color GRAY       = { 130, 130, 130, 255 };  // Gray
const Color DARKGRAY   = {  80,  80,  80, 255 };  // Dark Gray
const Color YELLOW     = { 253, 249,   0, 255 };  // Yellow
const Color GOLD       = { 255, 203,   0, 255 };  // Gold
const Color ORANGE     = { 255, 161,   0, 255 };  // Orange
const Color PINK       = { 255, 109, 194, 255 };  // Pink
const Color RED        = { 230,  41,  55, 255 };  // Red
const Color MAROON     = { 190,  33,  55, 255 };  // Maroon
const Color GREEN      = {   0, 228,  48, 255 };  // Green
const Color LIME       = {   0, 158,  47, 255 };  // Lime
const Color DARKGREEN  = {   0, 117,  44, 255 };  // Dark Green
const Color SKYBLUE    = { 102, 191, 255, 255 };  // Sky Blue
const Color BLUE       = {   0, 121, 241, 255 };  // Blue
const Color DARKBLUE   = {   0,  82, 172, 255 };  // Dark Blue
const Color PURPLE     = { 200, 122, 255, 255 };  // Purple
const Color VIOLET     = { 135,  60, 190, 255 };  // Violet
const Color DARKPURPLE = { 112,  31, 126, 255 };  // Dark Purple
const Color BEIGE      = { 211, 176, 131, 255 };  // Beige
const Color BROWN      = { 127, 106,  79, 255 };  // Brown
const Color DARKBROWN  = {  76,  63,  47, 255 };  // Dark Brown
const Color WHITE      = { 255, 255, 255, 255 };  // White
const Color BLACK      = {   0,   0,   0, 255 };  // Black
const Color BLANK      = {   0,   0,   0,   0 };  // Blank (Transparent)
const Color MAGENTA    = { 255,   0, 255, 255 };  // Magenta
const Color RAYWHITE   = { 245, 245, 245, 255 };  // My own White (raylib logo)

void CreateWindow();
void UpdateWindow();
void DestroyWindow();
bool WindowQuit();

Vector2 GetWasdInput();
Vector2 GetArrowInput();
Vector2 GetMouseInput();

void DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Color color);
void DrawLine(Vector3 v1, Vector3 v2, Color color);
void DrawGroundPlane(Vector3 position, Vector2 size, Color color);
void DrawSphere2(Vector3 centerPos, float radius, Color color);
void DrawSphereWire(Vector3 position, float radius, Color color);
void DrawBound(Vector3 position, Vector3 size, Color color);
void DrawCylinder(Vector3 position, float radius, float height, Color color);

void StartMode3d(Vector3 position, Vector3 target, Vector3 up, float fovy, int projection);
void FinishMode3d();

void BeginDraw(int cpu);
void EndDraw();

void DrawBox(Vector3 position, Vector3 rotation, Vector3 halfs, Color color, Color colorWire)
{
    Vector3 v0 = { -halfs.x, -halfs.y, -halfs.z };
    Vector3 v1 = { -halfs.x, -halfs.y,  halfs.z };
    Vector3 v2 = { -halfs.x,  halfs.y, -halfs.z };
    Vector3 v3 = { -halfs.x,  halfs.y,  halfs.z };
    Vector3 v4 = {  halfs.x, -halfs.y, -halfs.z };
    Vector3 v5 = {  halfs.x, -halfs.y,  halfs.z };
    Vector3 v6 = {  halfs.x,  halfs.y, -halfs.z };
    Vector3 v7 = {  halfs.x,  halfs.y,  halfs.z };

    auto rotMat = MatrixRotate(rotation);

    v0 *= rotMat;
    v1 *= rotMat;
    v2 *= rotMat;
    v3 *= rotMat;
    v4 *= rotMat;
    v5 *= rotMat;
    v6 *= rotMat;
    v7 *= rotMat;

    v0 += position;
    v1 += position;
    v2 += position;
    v3 += position;
    v4 += position;
    v5 += position;
    v6 += position;
    v7 += position;

    DrawTriangle(v1, v3, v2, color);
    DrawTriangle(v1, v2, v0, color);
    DrawTriangle(v4, v6, v7, color);
    DrawTriangle(v4, v7, v5, color);
    DrawTriangle(v0, v2, v6, color);
    DrawTriangle(v0, v6, v4, color);
    DrawTriangle(v2, v3, v7, color);
    DrawTriangle(v2, v7, v6, color);
    DrawTriangle(v1, v0, v4, color);
    DrawTriangle(v1, v4, v5, color);
    DrawTriangle(v5, v7, v3, color);
    DrawTriangle(v5, v3, v1, color);

    DrawLine(v0, v1, colorWire);
    DrawLine(v1, v5, colorWire);
    DrawLine(v5, v4, colorWire);
    DrawLine(v4, v0, colorWire);
    DrawLine(v2, v3, colorWire);
    DrawLine(v3, v7, colorWire);
    DrawLine(v7, v6, colorWire);
    DrawLine(v6, v2, colorWire);
    DrawLine(v0, v2, colorWire);
    DrawLine(v4, v6, colorWire);
    DrawLine(v1, v3, colorWire);
    DrawLine(v5, v7, colorWire);
}
void DrawBoxWire(Vector3 position, Vector3 rotation, Vector3 halfs, Color color)
{
    Vector3 v0 = { -halfs.x, -halfs.y, -halfs.z };
    Vector3 v1 = { -halfs.x, -halfs.y,  halfs.z };
    Vector3 v2 = { -halfs.x,  halfs.y, -halfs.z };
    Vector3 v3 = { -halfs.x,  halfs.y,  halfs.z };
    Vector3 v4 = {  halfs.x, -halfs.y, -halfs.z };
    Vector3 v5 = {  halfs.x, -halfs.y,  halfs.z };
    Vector3 v6 = {  halfs.x,  halfs.y, -halfs.z };
    Vector3 v7 = {  halfs.x,  halfs.y,  halfs.z };

    auto rotMat = MatrixRotate(rotation);

    v0 *= rotMat;
    v1 *= rotMat;
    v2 *= rotMat;
    v3 *= rotMat;
    v4 *= rotMat;
    v5 *= rotMat;
    v6 *= rotMat;
    v7 *= rotMat;

    v0 += position;
    v1 += position;
    v2 += position;
    v3 += position;
    v4 += position;
    v5 += position;
    v6 += position;
    v7 += position;

    DrawLine(v0, v1, color);
    DrawLine(v1, v5, color);
    DrawLine(v5, v4, color);
    DrawLine(v4, v0, color);
    DrawLine(v2, v3, color);
    DrawLine(v3, v7, color);
    DrawLine(v7, v6, color);
    DrawLine(v6, v2, color);
    DrawLine(v0, v2, color);
    DrawLine(v4, v6, color);
    DrawLine(v1, v3, color);
    DrawLine(v5, v7, color);
}
void DrawBoxTest(Vector3 position, Vector3 rotation, Vector3 halfs, Vector3 offsetPosition, Color color, Color colorWire)
{
    Vector3 v0 = { -halfs.x, -halfs.y, -halfs.z };
    Vector3 v1 = { -halfs.x, -halfs.y,  halfs.z };
    Vector3 v2 = { -halfs.x,  halfs.y, -halfs.z };
    Vector3 v3 = { -halfs.x,  halfs.y,  halfs.z };
    Vector3 v4 = {  halfs.x, -halfs.y, -halfs.z };
    Vector3 v5 = {  halfs.x, -halfs.y,  halfs.z };
    Vector3 v6 = {  halfs.x,  halfs.y, -halfs.z };
    Vector3 v7 = {  halfs.x,  halfs.y,  halfs.z };

    v0 += offsetPosition;
    v1 += offsetPosition;
    v2 += offsetPosition;
    v3 += offsetPosition;
    v4 += offsetPosition;
    v5 += offsetPosition;
    v6 += offsetPosition;
    v7 += offsetPosition;

    auto rotMat = MatrixRotate(rotation);

    v0 *= rotMat;
    v1 *= rotMat;
    v2 *= rotMat;
    v3 *= rotMat;
    v4 *= rotMat;
    v5 *= rotMat;
    v6 *= rotMat;
    v7 *= rotMat;

    v0 += position;
    v1 += position;
    v2 += position;
    v3 += position;
    v4 += position;
    v5 += position;
    v6 += position;
    v7 += position;

    DrawTriangle(v1, v3, v2, color);
    DrawTriangle(v1, v2, v0, color);
    DrawTriangle(v4, v6, v7, color);
    DrawTriangle(v4, v7, v5, color);
    DrawTriangle(v0, v2, v6, color);
    DrawTriangle(v0, v6, v4, color);
    DrawTriangle(v2, v3, v7, color);
    DrawTriangle(v2, v7, v6, color);
    DrawTriangle(v1, v0, v4, color);
    DrawTriangle(v1, v4, v5, color);
    DrawTriangle(v5, v7, v3, color);
    DrawTriangle(v5, v3, v1, color);

    DrawLine(v0, v1, colorWire);
    DrawLine(v1, v5, colorWire);
    DrawLine(v5, v4, colorWire);
    DrawLine(v4, v0, colorWire);
    DrawLine(v2, v3, colorWire);
    DrawLine(v3, v7, colorWire);
    DrawLine(v7, v6, colorWire);
    DrawLine(v6, v2, colorWire);
    DrawLine(v0, v2, colorWire);
    DrawLine(v4, v6, colorWire);
    DrawLine(v1, v3, colorWire);
    DrawLine(v5, v7, colorWire);
}
void DrawBox(Vector3 position, Vector3 rotation, Vector3 halfs, Color color)
{
    Vector3 v0 = { -halfs.x, -halfs.y, -halfs.z };
    Vector3 v1 = { -halfs.x, -halfs.y,  halfs.z };
    Vector3 v2 = { -halfs.x,  halfs.y, -halfs.z };
    Vector3 v3 = { -halfs.x,  halfs.y,  halfs.z };
    Vector3 v4 = {  halfs.x, -halfs.y, -halfs.z };
    Vector3 v5 = {  halfs.x, -halfs.y,  halfs.z };
    Vector3 v6 = {  halfs.x,  halfs.y, -halfs.z };
    Vector3 v7 = {  halfs.x,  halfs.y,  halfs.z };

    auto rotMat = MatrixRotate(rotation);

    v0 *= rotMat;
    v1 *= rotMat;
    v2 *= rotMat;
    v3 *= rotMat;
    v4 *= rotMat;
    v5 *= rotMat;
    v6 *= rotMat;
    v7 *= rotMat;

    v0 += position;
    v1 += position;
    v2 += position;
    v3 += position;
    v4 += position;
    v5 += position;
    v6 += position;
    v7 += position;

    DrawTriangle(v1, v3, v2, color);
    DrawTriangle(v1, v2, v0, color);
    DrawTriangle(v4, v6, v7, color);
    DrawTriangle(v4, v7, v5, color);
    DrawTriangle(v0, v2, v6, color);
    DrawTriangle(v0, v6, v4, color);
    DrawTriangle(v2, v3, v7, color);
    DrawTriangle(v2, v7, v6, color);
    DrawTriangle(v1, v0, v4, color);
    DrawTriangle(v1, v4, v5, color);
    DrawTriangle(v5, v7, v3, color);
    DrawTriangle(v5, v3, v1, color);
}
void DrawAxis(Vector3 position, Vector3 rotation, float scale)
{
    auto origin = Vector3Zero();
    auto right = Vector3Right();
    auto up = Vector3Up();
    auto forward = Vector3Forward();

    right   *= scale;
    up      *= scale;
    forward *= scale;

    auto rotMat = MatrixRotate(rotation);

    right   *= rotMat;
    up      *= rotMat;
    forward *= rotMat;

    origin  += position;
    right   += position;
    up      += position;
    forward += position;

    DrawLine(origin, right,   RED);
    DrawLine(origin, up,      GREEN);
    DrawLine(origin, forward, BLUE);
}
void DrawCube(Vector3 position, Vector3 size, Color color)
{
    position.x = -position.x;
    DrawBound(position, size, color);
}
void DrawBound(const Bound& bound, Color color)
{
    auto center = BoundCenter(bound);
    auto size = BoundSize(&bound);
    DrawCube(center, size, WHITE);
}

void RenderAxisCubes()
{
    DrawCube({ 10, 0, 0 }, { 1, 1, 1 }, RED);
    DrawCube({ 0, 10, 0 }, { 1, 1, 1 }, GREEN);
    DrawCube({ 0, 0, 10 }, { 1, 1, 1 }, BLUE);
    DrawCube({ -10, 0, 0 }, { 1, 1, 1 }, VIOLET);
    DrawCube({ 0, -10, 0 }, { 1, 1, 1 }, YELLOW);
    DrawCube({ 0, 0, -10 }, { 1, 1, 1 }, ORANGE);
}
void DrawBoxGreen(const Box& box)
{
    DrawBox(box.position, box.rotation, box.halfExtent, LIME, RED);
}
void DrawBoxBlue(const Box& box)
{
    DrawBox(box.position, box.rotation, box.halfExtent, BLUE, SKYBLUE);
}

void StartMode3d(Vector3 position, float yaw, float pitch)
{
    Vector3 up      = { 0, 1, 0 };
    Vector3 forward = { 0, 0, 1 };

    // pitch = MathToRadians(pitch);
    // yaw = MathToRadians(yaw);

    // TODO
    // up = Vector3RotateY(up, pitch);
    // up = Vector3RotateX(up, yaw);
    // up = { 0, cos(pitch), 0};
    // up = Vector3Normalize(up);

    forward = Vector3RotateX(forward, pitch);
    forward = Vector3RotateY(forward, -yaw);
    up = Vector3RotateX(up, pitch);
    up = Vector3RotateY(up, -yaw);

    auto target = Vector3Add(position, forward);
    auto fovy = 90;
    auto projection = 0;

    position.x = -position.x;
    target.x = -target.x;
    up.x = -up.x;

    StartMode3d(position, target, up, fovy, projection);
}
void StartMode3d(const Camera& camera)
{
    StartMode3d(camera.position, camera.yaw, camera.pitch);
}

bool IsKeyPressedRl(int key);
bool IsKeyDownRl(int key);