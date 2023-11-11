#pragma once

#include "Boid.h"
#include "BoidWorld.h"

class World
{
public:
    Camera camera;
    BoidWorld boidWorld;

    World()
    {
        camera.position += Vector3Up()*50;
        camera.position += Vector3Backward()*200;
    }

    // TODO maybe do something about that
    void Update(float deltaTime,
        bool left, bool up, bool down, bool right,
        bool w, bool a, bool s, bool d, bool e, bool q)
    {
        UpdateCameraPosition(&camera, deltaTime, w, a, s, d, e, q);
        UpdateCameraRotation(&camera, deltaTime, left, up, down, right);
        boidWorld.Update(deltaTime);
    }
};