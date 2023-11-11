#include "../Base/Base.h"
#include "../Boids/World.h"
#include "../Renderer/Renderer.h"

void main2()
{
    auto world = make_unique<World>();

    HINSTANCE hInstance = GetModuleHandle(nullptr);
    auto renderer = make_unique<Renderer>(hInstance);
    renderer->Render(&world->camera, world->boidWorld.boids);

    while (true)
    {
        world->Update(0.02f,
            renderer->Keydown_VK_LEFT(),
            renderer->Keydown_VK_UP(),
            renderer->Keydown_VK_DOWN(),
            renderer->Keydown_VK_RIGHT(),
            renderer->Keydown_W(),
            renderer->Keydown_A(),
            renderer->Keydown_S(),
            renderer->Keydown_D(),
            renderer->Keydown_E(),
            renderer->Keydown_Q());

        renderer->Render(&world->camera, world->boidWorld.boids);
        if (renderer->WindowIsClosed())
            break;
    }
}

int main()
{
    try
    {
        main2();
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}